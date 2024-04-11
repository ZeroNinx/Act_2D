#include "Monster/MonsterSkillComponent.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

UMonsterSkillComponent::UMonsterSkillComponent()
{
	//开启Tick
	PrimaryComponentTick.bCanEverTick = true;

	//设置重叠
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	//设置Sprite不可见
	SetVisibility(false);

	//初始化OwingMonster
	AMonster* Monster = Cast<AMonster>(GetOwner());
	if (Monster)
	{
		this->OwingMonster = Monster;
	}
	else
	{
		UGlobalBlueprintFunctionLibrary::LogWarning("UMonsterSkillComponent OwingMonster Setup Failed");
	}
}

void UMonsterSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 当前技能有效
	if (CurrentSkill)
	{
		if (!bSkillJudgeBegin && !bSkillJudgeEnd) // 技能判定未开始
		{
			CurrentSkill->TickBeforeSkillJudge();
		}
		else if (bSkillJudgeBegin && !bSkillJudgeEnd) // 技能判定进行中
		{
			CurrentSkill->TickOnSkillJudge();
		}
		else if (bSkillJudgeBegin && bSkillJudgeEnd) // 技能判定已结束
		{
			CurrentSkill->TickAfterSkillJudge();
		}
	}

}

void UMonsterSkillComponent::UseSkill(UMonsterSkill* NewSkill)
{
	if (!NewSkill)
	{
		return;
	}

	ForceSkillEnd();
	CurrentSkill = NewSkill;
	SetupSkill();
	SkillBegin();
}

void UMonsterSkillComponent::SetupSkill()
{
	if (!CurrentSkill)
	{
		return;
	}

	CurrentSkill->OwingMonster = OwingMonster;

	//设定攻击动画
	UPaperZDAnimSequence* AttackAnimSeq = CurrentSkill->AttackOverrideSequence;
	if (AttackAnimSeq)
	{
		OwingMonster->PlayOverrideAnim(AttackAnimSeq);
	}

	//设置当前的判定范围
	UPaperSprite* AttackSprite = CurrentSkill->AttackJudgeSprite;
	if (AttackSprite)
	{
		SetSprite(AttackSprite);
	}
}

void UMonsterSkillComponent::ResetSkill()
{
	bSkillJudgeBegin = false;
	bSkillJudgeEnd = false;
	CurrentSkill = nullptr;
}

void UMonsterSkillComponent::SetSkillJudgeBegin()
{
	bSkillJudgeBegin = true;
	SkillJudgeBegin();
}

void UMonsterSkillComponent::SetSkillJudgeEnd()
{
	bSkillJudgeEnd = true;
	SkillJudgeEnd();
}

void UMonsterSkillComponent::SetSkillEnd()
{
	if (CurrentSkill)
	{
		// 持续性技能不会被停止，强制停止请使用ForceSkillEnd
		if (!CurrentSkill->SkillProperty.bPersistent)
		{
			ForceSkillEnd();
		}
	}

}

void UMonsterSkillComponent::ForceSkillEnd()
{
	SkillEnd();
	ResetSkill();
}

void UMonsterSkillComponent::SkillBegin()
{
	if (CurrentSkill)
	{
		CurrentSkill->OnSkillBegin();
		OnSkillBegin.Broadcast();
	}
}

void UMonsterSkillComponent::SkillJudgeBegin()
{
	if (CurrentSkill)
	{
		CurrentSkill->OnSkillJudgeBegin();
		OnSkillJudgeBegin.Broadcast();
	}
}

void UMonsterSkillComponent::SkillJudgeEnd()
{
	if (CurrentSkill)
	{
		CurrentSkill->OnSkillJudgeEnd();
		OnSkillJudgeEnd.Broadcast();
	}
}

void UMonsterSkillComponent::SkillEnd()
{
	if (CurrentSkill)
	{
		CurrentSkill->OnSkillEnd();
		OnSkillEnd.Broadcast();
	}
}
