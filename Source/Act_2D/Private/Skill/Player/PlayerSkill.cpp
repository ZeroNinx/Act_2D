#include "Skill/Player/PlayerSkill.h"
#include "Monster/Monster.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerAttackComponent.h"
#include "Utils/ActorInterface.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

UPlayerSkill::UPlayerSkill()
{
	
}

//执行攻击
void UPlayerSkill::ExecuteHit_Implementation(AActor* HitActor)
{
	APlayerCharacter* PlayerCharacter = UGlobalBlueprintFunctionLibrary::GetPlayerCharacter();
	IActorInterface::Execute_Hit(HitActor, PlayerCharacter, SkillProperty);

	//打击感延迟
	AMonster* HitMonster = Cast<AMonster>(HitActor);
	if (HitMonster && HitMonster->IsAlive())
	{
		UGlobalBlueprintFunctionLibrary::SetGlobalDelay(0.05f, 0.07f);
	}
}

TArray<AActor*> UPlayerSkill::GetHitActors()
{
	TArray<AActor*> HitActors;
	APlayerCharacter* PlayerCharacter = UGlobalBlueprintFunctionLibrary::GetPlayerCharacter();
	if (PlayerCharacter)
	{
		PlayerCharacter->GetAttackComponent()->UpdateOverlaps();
		PlayerCharacter->GetAttackComponent()->GetOverlappingActors(HitActors);
	}
	return MoveTemp(HitActors);
}

void UPlayerSkill::OnSkillBegin_Implementation()
{

}

void UPlayerSkill::TickBeforeSkillJudge_Implementation()
{
	if (SkillProperty.bEnableExecuteHitOnTick && SkillProperty.ExecuteHitOnTickBeforeJudge)
	{
		HitPerActorOnce();
	}
}

void UPlayerSkill::OnSkillJudgeBegin_Implementation()
{
	HitPerActorOnce();
}

void UPlayerSkill::TickOnSkillJudge_Implementation()
{
	if (SkillProperty.bEnableExecuteHitOnTick && SkillProperty.ExecuteHitOnJudgeTick)
	{
		HitPerActorOnce();
	}
}

void UPlayerSkill::OnSkillJudgeEnd_Implementation()
{

}

void UPlayerSkill::TickAfterSkillJudge_Implementation()
{
	if (SkillProperty.bEnableExecuteHitOnTick && SkillProperty.ExecuteHitOnTickAfterJudge)
	{
		HitPerActorOnce();
	}
}

void UPlayerSkill::OnSkillEnd_Implementation()
{

}

void UPlayerSkill::HitPerActorOnce()
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

