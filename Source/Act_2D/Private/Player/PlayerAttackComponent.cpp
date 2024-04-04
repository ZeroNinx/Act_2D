#include "Player/PlayerAttackComponent.h"

//互相包含
#include "Monster/Monster.h"
#include "Skill/Player/PlayerSkill.h"
#include "Player/PlayerCharacterController.h"
#include "Utils/StateMachine.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"
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
		this->PlayerCharacter = Character;
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
	if (!IsAttacking())
	{
		// 获取下一个组合输入
		FKeyCombination NextCombation = GetNextKeyCombination();
		if(NextCombation.IsEmpty())
		{
			return;
		}

		//跳跃攻击
		//TODO:移植到新方式
		bool bPlayerJumping = !PlayerCharacter->IsMovingOnGround && !(PlayerCharacter->GetState() == EState::Hit);
		if (bPlayerJumping)
		{
			//TODO:跳跃攻击
		}
		else //一般攻击
		{
			CurrentSkill = GetNextSkill(NextCombation);
			if (CurrentSkill)
			{
				PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
				Attack();
			}
		}
	}

	//当处于攻击时
	if (IsAttacking() && CurrentSkill)
	{
		if (!bPlayerAttackJudgeBegin && !bPlayerAttackJudgeEnd) // 攻击判定未开始
		{
			CurrentSkill->TickBeforeAttackJudge();
		}
		else if (bPlayerAttackJudgeBegin && !bPlayerAttackJudgeEnd) // 攻击判定进行中
		{
			CurrentSkill->TickOnAttackJudge();
		}
		else if (bPlayerAttackJudgeBegin && bPlayerAttackJudgeEnd) // 攻击判定已结束
		{

			// 获取下一个组合输入
			FKeyCombination NextCombation = GetNextKeyCombination();
			if (NextCombation.IsEmpty())
			{
				return;
			}

			UPlayerSkill* NextSkill = GetNextSkill(NextCombation);
			if(NextSkill)
			{
				ResetAttack();
				CurrentSkill = NextSkill;
				Attack();
			}
			else
			{
				// 没有下一个连续技
				CurrentSkill->TickAfterAttackJudge();
			}
		}
	}
}

FKeyCombination UPlayerAttackComponent::GetNextKeyCombination()
{
	if (PlayerCharacter)
	{
		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			return PlayerController->GetNextKeyCombation();
		}
	}
	return FKeyCombination();
}

bool UPlayerAttackComponent::IsAttacking()
{
	return PlayerCharacter->GetState() == EState::Attack;
}

//返回是否可以移动
bool UPlayerAttackComponent::IsMovable()
{
	return !IsAttacking()  || (bPlayerAttackJudgeBegin && bPlayerAttackJudgeEnd);
}

void UPlayerAttackComponent::PlayerAttackBegin()
{
	CurrentSkill->OnAttackBegin();
	OnPlayerAttackBegin.Broadcast();
}

void UPlayerAttackComponent::PlayerAttackJudgeBegin()
{
	CurrentSkill->OnAttackJudgeBegin();
	OnPlayerAttackJudgeBegin.Broadcast();
}

void UPlayerAttackComponent::PlayerAttacJudgeEnd()
{
	CurrentSkill->OnAttackJudgeEnd();
	OnPlayerAttackJudgeEnd.Broadcast();
}

void UPlayerAttackComponent::PlayerAttackEnd()
{
	ResetAttack();
	OnPlayerAttackEnd.Broadcast();
}

UPlayerSkill* UPlayerAttackComponent::GetNextSkill(FKeyCombination KeyCombination)
{
	// 不在攻击，从基础配置里读取
	if (!IsAttacking())
	{
		TSubclassOf<UPlayerSkill> SkillClass = nullptr;
		
		// 先找完全匹配的
		if (BasicSkillConfig.Contains(KeyCombination))
		{
			SkillClass = BasicSkillConfig[KeyCombination];
		}
		// 再找包含的
		for (auto It : BasicSkillConfig)
		{
			if (KeyCombination.ContainsCombination(It.Key))
			{
				SkillClass = It.Value;
			}
		}

		if (SkillClass)
		{
			// TODO:优化载入
			return NewObject<UPlayerSkill>(this, SkillClass);
		}
	}

	// 当前已有技能，读取下一个组合
	if (IsAttacking() && CurrentSkill)
	{
		TSubclassOf<UPlayerSkill> SkillClass = nullptr;

		// 先找完全匹配的
		if (CurrentSkill->ComboConfig.Contains(KeyCombination))
		{
			SkillClass = CurrentSkill->ComboConfig[KeyCombination];
		}
		// 再找包含的
		for (auto It : CurrentSkill->ComboConfig)
		{
			if (KeyCombination.ContainsCombination(It.Key))
			{
				SkillClass = It.Value;
			}
		}

		if (SkillClass)
		{
			// TODO:优化载入
			return NewObject<UPlayerSkill>(this, SkillClass);
		}
	}

	return nullptr;
}

//攻击
void UPlayerAttackComponent::Attack()
{
	if (!CurrentSkill)
	{
		return;
	}

	UGlobalBlueprintFunctionLibrary::LogWarning("Attack!!!!!!!!");

	//改变状态为攻击
	PlayerCharacter->SetState(EState::Attack);

	//初始化攻击资源
	SetupAttack();

	//开始攻击
	PlayerAttackBegin();
}

//初始化攻击
void UPlayerAttackComponent::SetupAttack()
{
	if (!CurrentSkill)
	{
		return;
	}

	UPaperZDAnimSequence* AttackAnimSeq = CurrentSkill->AttackOverrideSequence;
	if (!AttackAnimSeq)
	{
		UGlobalBlueprintFunctionLibrary::LogWarning("UPlayerAttackComponent Load AttackAnimSeq Failed");
		return;
	}

	UPaperSprite* AttackSprite = CurrentSkill->AttackJudgeSprite;
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
	bPlayerAttackJudgeBegin = false;
	bPlayerAttackJudgeEnd = false;
	CurrentSkill = nullptr;
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

void UPlayerAttackComponent::SetPlayerAttackEnd()
{
	PlayerAttackEnd();
}

