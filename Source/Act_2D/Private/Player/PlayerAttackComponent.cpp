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

	//隐藏攻击模块
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
	if (AttackID == 0 && !NextKeyCombation.IsAttackEmpty())
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
	if (AttackID != 0)
	{	
		//根据动画播放位置（攻击进行的阶段）触发不同逻辑
		int AnimationPosition = GetAnimationPosition();

		if (AnimationPosition < AttackStartFrame)
		{
			//攻击前摇帧
			if (bBeforeAttackShouldJudge)
			{
				bBeforeAttackShouldJudge = false;
				Skill->JudgeAtBeginAttack(PlayerCharacter);
			}
			Skill->JudgeDuringBeforeAttack(PlayerCharacter);
		}
		else if (AnimationPosition >= AttackStartFrame && AnimationPosition < AttackFinishFrame)
		{
			//攻击进行帧
			if (bInAttackShouldJudge)
			{
				bInAttackShouldJudge = false;
				Skill->JudgeAtStartAttack(PlayerCharacter);
			}
			Skill->JudgeDuringAttack(PlayerCharacter);
		}
		else if (AnimationPosition > AttackFinishFrame)
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

			//攻击后摇帧
			if (bAfterAttackShouldJudge)
			{
				bAfterAttackShouldJudge = false;
				Skill->JudgeAtFinishAttack(PlayerCharacter);
			}
			Skill->JudgeDuringFinishAttack(PlayerCharacter);
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
	return AttackID == 0 || GetAnimationPosition() >= AttackFinishFrame;
}

//接收下一次攻击组合
void UPlayerAttackComponent::SetKeyCombination(FKeyCombination KeyCombation)
{
	if (IsMovable() && !KeyCombation.IsAttackEmpty())
	{
		NextKeyCombation = KeyCombation;
	}
}

//获得当前攻击动画播放位置
int UPlayerAttackComponent::GetAnimationPosition()
{
	return PlayerCharacter->GetSprite()->GetPlaybackPositionInFrames();
}


//攻击
void UPlayerAttackComponent::Attack(int ID)
{
	//改变状态为攻击
	PlayerCharacter->SetState(EState::Attack);

	//使动画结束播放的代理得以触发
	PlayerCharacter->GetSprite()->SetLooping(false);

	//设定攻击ID和类型
	AttackID = ID;
	SwitchAttack();

	//初始化攻击资源
	SetupAttack();

	//初始化连续技资源
	SetupCombo();

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

	//设置当前的判定范围和帧
	SetSprite(AttackSprite);
	/*AttackStartFrame = attack_start_frame;
	AttackFinishFrame = attack_finish_frame;*/

}

//重置攻击
void UPlayerAttackComponent::ResetAttack()
{
	AttackID = 0;
	AttackStartFrame = 0;
	AttackFinishFrame = 0;
	bAfterAttackShouldJudge = true;
	bInAttackShouldJudge = true;
	bAfterAttackShouldJudge = true;
	NextKeyCombation.Clear();
	Skill = NewObject<USkill>();
}

