#include "Monster/MonsterSkillComponent.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

UMonsterSkillComponent::UMonsterSkillComponent()
{
	//开启Tick
	PrimaryComponentTick.bCanEverTick = true;

	//设置重叠
	SetCollisionResponseToAllChannels(ECR_Ignore);
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
	ResetSkill();
	CurrentSkill = NewSkill;
	SkillBegin();
}

void UMonsterSkillComponent::SetupSkill()
{
	if (!CurrentSkill)
	{
		return;
	}

	UPaperZDAnimSequence* AttackAnimSeq = CurrentSkill->AttackOverrideSequence;
	if (!AttackAnimSeq)
	{
		UGlobalBlueprintFunctionLibrary::LogWarning("UMonsterSkillComponent Load AttackAnimSeq Failed");
		return;
	}

	UPaperSprite* AttackSprite = CurrentSkill->AttackJudgeSprite;
	if (!AttackSprite)
	{
		UGlobalBlueprintFunctionLibrary::LogWarning("UMonsterSkillComponent Load AttackSprite Failed");
		return;
	}

	//设定攻击动画
	OwingMonster->PlayOverrideAnim(AttackAnimSeq);

	//设置当前的判定范围
	SetSprite(AttackSprite);
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

void UMonsterSkillComponent::SetPlayerSkillEnd()
{
	SkillEnd();
	ResetSkill();
}

void UMonsterSkillComponent::SkillBegin()
{
	CurrentSkill->OnSkillBegin();
	OnSkillBegin.Broadcast();
}

void UMonsterSkillComponent::SkillJudgeBegin()
{
	CurrentSkill->OnSkillJudgeBegin();
	OnSkillJudgeBegin.Broadcast();
}

void UMonsterSkillComponent::SkillJudgeEnd()
{
	CurrentSkill->OnSkillJudgeEnd();
	OnSkillJudgeEnd.Broadcast();
}

void UMonsterSkillComponent::SkillEnd()
{
	CurrentSkill->OnSkillEnd();
	OnSkillEnd.Broadcast();
}
