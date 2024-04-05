#include "Skill/Player/PlayerSkill.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

//循环包含
#include "Player/PlayerAttackComponent.h"
#include "Player/PlayerCharacter.h"

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

}

void UPlayerSkill::OnSkillJudgeBegin_Implementation()
{
	for (AActor* HitActor : GetHitActors())
	{
		ExecuteHit(HitActor);
	}
}

void UPlayerSkill::TickOnSkillJudge_Implementation()
{

}

void UPlayerSkill::OnSkillJudgeEnd_Implementation()
{

}

void UPlayerSkill::TickAfterSkillJudge_Implementation()
{

}

void UPlayerSkill::OnSkillEnd_Implementation()
{

}

