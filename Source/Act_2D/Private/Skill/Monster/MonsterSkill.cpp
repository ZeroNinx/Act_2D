#include "Skill/Monster/MonsterSkill.h"
#include "Player/PlayerCharacter.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"
#include "Utils/ActorInterface.h"

UMonsterSkill::UMonsterSkill()
{
	
}

void UMonsterSkill::ExecuteHit_Implementation(AActor* HitActor)
{
	if (HitActor->IsA<AMonster>())
	{
		return;
	}
	IActorInterface::Execute_Hit(HitActor, OwingMonster, SkillProperty);
}

TArray<AActor*> UMonsterSkill::GetHitActors()
{
	TArray<AActor*> HitActors;
	if (OwingMonster)
	{
		OwingMonster->GetSkillComponent()->UpdateOverlaps();
		OwingMonster->GetSkillComponent()->GetOverlappingActors(HitActors);
	}
	return MoveTemp(HitActors);
}

void UMonsterSkill::OnSkillBegin_Implementation()
{

}

void UMonsterSkill::TickBeforeSkillJudge_Implementation()
{
	if (SkillProperty.bEnableExecuteHitOnTick && SkillProperty.ExecuteHitOnTickBeforeJudge)
	{
		HitPerActorOnce();
	}
}

void UMonsterSkill::OnSkillJudgeBegin_Implementation()
{
	HitPerActorOnce();
}

void UMonsterSkill::TickOnSkillJudge_Implementation()
{
	if (SkillProperty.bEnableExecuteHitOnTick && SkillProperty.ExecuteHitOnJudgeTick)
	{
		HitPerActorOnce();
	}
}

void UMonsterSkill::OnSkillJudgeEnd_Implementation()
{

}

void UMonsterSkill::TickAfterSkillJudge_Implementation()
{
	if (SkillProperty.bEnableExecuteHitOnTick && SkillProperty.ExecuteHitOnTickAfterJudge)
	{
		HitPerActorOnce();
	}
}

void UMonsterSkill::OnSkillEnd_Implementation()
{

}

void UMonsterSkill::HitPerActorOnce()
{
	TArray<AActor*>&& HitActors = GetHitActors();
	for (AActor* Actor : HitActors)
	{
		if (!AlreadyHitActors.Contains(Actor))
		{
			ExecuteHit(Actor);
			AlreadyHitActors.Add(Actor);
		}
	}
}
