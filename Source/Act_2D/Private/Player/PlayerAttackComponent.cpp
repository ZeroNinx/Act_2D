#include "PlayerAttackComponent.h"

//互相包含
#include "Monster.h"
#include "PlayerSkill.h"
#include "StateMachine.h"
#include "PlayerCharacterController.h"
#include "GlobalBlueprintFunctionLibrary.h"
#include "AnimSequences/PaperZDAnimSequence.h"

//构造函数
UPlayerAttackComponent::UPlayerAttackComponent()
{
	//启用tick
	PrimaryComponentTick.bCanEverTick = true;

	//设置重叠
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	//设置Sprite不可见
	SetVisibility(false);

	//初始化Character
	APlayerCharacter* Character = Cast<APlayerCharacter>(GetOwner());
	if (Character)
	{
		Setup(Character);
	}
	else
	{
		UGlobalBlueprintFunctionLibrary::LogWarning("UPlayerAttackComponent PlayerCharacter Setup Failed");
	}
}

//Tick函数
void UPlayerAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//当攻击外进入攻击
	if (!IsAttacking() && !NextKeyCombation.IsAttackEmpty())
	{
		//跳跃攻击
		bool bPlayerJumping = PlayerCharacter->IsInState(EState::Jump|EState::Fall);
		if (bPlayerJumping)
		{
			Attack(4);
		}
		else //一般攻击
		{
			//获得命令
			int NextCommand = NextKeyCombation.GetCommand();
			NextKeyCombation.Clear();

			//进行对应攻击
			SetupCombo();
			if (ComboMap.Contains(NextCommand))
			{
				//立即停止移动，开始攻击
				PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
				Attack(ComboMap[NextCommand]);
			}
		}
	}

	//当处于攻击时
	if (IsAttacking())
	{	
		if (!bPlayerAttackJudgeBegin && !bPlayerAttackJudgeEnd) // 攻击判定未开始
		{
			Skill->TickBeforeAttackJudge();
		}
		else if (bPlayerAttackJudgeBegin && !bPlayerAttackJudgeEnd) // 攻击判定进行中
		{
			Skill->TickOnAttackJudge();
		}
		else if (bPlayerAttackJudgeBegin && bPlayerAttackJudgeEnd) // 攻击判定已结束
		{
			//若有指令，进行指令判定
			if (!NextKeyCombation.IsAttackEmpty())
			{
				int NextCommand = NextKeyCombation.GetCommand();

				if (ComboMap.Contains(NextCommand))
				{
					ResetAttack();
					Attack(ComboMap[NextCommand]);
					return;
				}
			}

			Skill->TickAfterAttackJudge();
		}
	}
}

//初始化
void UPlayerAttackComponent::Setup(APlayerCharacter* NewCharacter)
{
	PlayerCharacter = NewCharacter;
}

bool UPlayerAttackComponent::IsAttacking()
{
	return AttackID != 0;
}

//返回是否可以移动
bool UPlayerAttackComponent::IsMovable()
{
	return !IsAttacking()  || (bPlayerAttackJudgeBegin && bPlayerAttackJudgeEnd);
}

//接收下一次攻击组合
void UPlayerAttackComponent::SetKeyCombination(FKeyCombination KeyCombation)
{
	if (IsMovable() && !KeyCombation.IsAttackEmpty())
	{
		NextKeyCombation = KeyCombation;
	}
}

void UPlayerAttackComponent::PlayerAttackBegin()
{
	Skill->OnAttackBegin();
	OnPlayerAttackBegin.Broadcast();
}

void UPlayerAttackComponent::PlayerAttackJudgeBegin()
{
	Skill->OnAttackJudgeBegin();
	OnPlayerAttackJudgeBegin.Broadcast();
}

void UPlayerAttackComponent::PlayerAttacJudgeEnd()
{
	Skill->OnAttackJudgeEnd();
	OnPlayerAttackJudgeEnd.Broadcast();
}

void UPlayerAttackComponent::PlayerAttackEnd()
{
	OnPlayerAttackEnd.Broadcast();
}

//攻击
void UPlayerAttackComponent::Attack(int ID)
{
	//改变状态为攻击
	PlayerCharacter->SetState(EState::Attack);

	//设定攻击ID和类型
	AttackID = ID;
	SwitchAttack();

	//初始化攻击资源
	SetupAttack();

	//初始化连续技资源
	SetupCombo();

	//开始攻击
	PlayerAttackBegin();
}

//选择攻击
void UPlayerAttackComponent::SwitchAttack()
{
	switch (AttackID)
	{
	case 1:
		Skill = NewObject<US_AttackI>(PlayerCharacter);
		break;
	case 2:
		Skill = NewObject<US_AttackII>(PlayerCharacter);
		break;
	case 3:
		Skill = NewObject<US_AttackIII>(PlayerCharacter);
		break;
	case 4:
		Skill = NewObject<US_AttackJump>(PlayerCharacter);
		break;
	case 5:
		Skill = NewObject<US_AttackDash>(PlayerCharacter);
		break;
	default:
		break;
	}
}

//初始化攻击
void UPlayerAttackComponent::SetupAttack()
{
	UPaperZDAnimSequence* AttackAnimSeq = GetAttackSequence();
	if (!AttackAnimSeq)
	{
		UGlobalBlueprintFunctionLibrary::LogWarning("UPlayerAttackComponent Load AttackAnimSeq Failed");
		return;
	}

	UPaperSprite* AttackSprite = GetAttackSprite();
	if (!AttackSprite)
	{
		UGlobalBlueprintFunctionLibrary::LogWarning("UPlayerAttackComponent Load AttackSprite Failed");
		return;
	}

	//设定攻击动画
	PlayerCharacter->PlayOverrideAnim(AttackAnimSeq);

	//设置当前的判定范围
	SetSprite(AttackSprite);

}

//重置攻击
void UPlayerAttackComponent::ResetAttack()
{
	AttackID = 0;
	bPlayerAttackJudgeBegin = false;
	bPlayerAttackJudgeEnd = false;
	NextKeyCombation.Clear();
	Skill = NewObject<UPlayerSkill>();
}

void UPlayerAttackComponent::SetPlayerAttackJudgeBegin()
{
	bPlayerAttackJudgeBegin = true;
	PlayerAttackJudgeBegin();
}

void UPlayerAttackComponent::SetPlayerAttackJudgeEnd()
{
	bPlayerAttackJudgeEnd = true;
	PlayerAttacJudgeEnd();
}

