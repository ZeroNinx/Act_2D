#include "Monster/MonsterSkillComponent.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

UMonsterSkillComponent::UMonsterSkillComponent()
{
	//����Tick
	PrimaryComponentTick.bCanEverTick = true;

	//�����ص�
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	//����Sprite���ɼ�
	SetVisibility(false);

	//��ʼ��OwingMonster
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

	// ��ǰ������Ч
	if (CurrentSkill)
	{
		if (!bSkillJudgeBegin && !bSkillJudgeEnd) // �����ж�δ��ʼ
		{
			CurrentSkill->TickBeforeSkillJudge();
		}
		else if (bSkillJudgeBegin && !bSkillJudgeEnd) // �����ж�������
		{
			CurrentSkill->TickOnSkillJudge();
		}
		else if (bSkillJudgeBegin && bSkillJudgeEnd) // �����ж��ѽ���
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

	//�趨��������
	OwingMonster->PlayOverrideAnim(AttackAnimSeq);

	//���õ�ǰ���ж���Χ
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
