#include "Monster/MonsterSkillComponent.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

UMonsterSkillComponent::UMonsterSkillComponent()
{
	//����Tick
	PrimaryComponentTick.bCanEverTick = true;

	//�����ص�
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
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

	//�趨��������
	UPaperZDAnimSequence* AttackAnimSeq = CurrentSkill->AttackOverrideSequence;
	if (AttackAnimSeq)
	{
		OwingMonster->PlayOverrideAnim(AttackAnimSeq);
	}

	//���õ�ǰ���ж���Χ
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
		// �����Լ��ܲ��ᱻֹͣ��ǿ��ֹͣ��ʹ��ForceSkillEnd
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
