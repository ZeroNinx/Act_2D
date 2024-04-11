#include "Skill/Monster/MonsterSkill.h"
#include "Player/PlayerCharacter.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"
#include "Utils/ActorInterface.h"

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

}

void UMonsterSkill::OnSkillJudgeBegin_Implementation()
{
	TArray<AActor*> HitActors = GetHitActors();
	for (AActor* Actor : HitActors)
	{
		ExecuteHit(Actor);
	}
}

void UMonsterSkill::TickOnSkillJudge_Implementation()
{

}

void UMonsterSkill::OnSkillJudgeEnd_Implementation()
{

}

void UMonsterSkill::TickAfterSkillJudge_Implementation()
{

}

void UMonsterSkill::OnSkillEnd_Implementation()
{

}
