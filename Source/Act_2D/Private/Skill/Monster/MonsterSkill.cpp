#include "Skill/Monster/MonsterSkill.h"
#include "Player/PlayerCharacter.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"
#include "Utils/ActorInterface.h"

void UMonsterSkill::ExecuteHit_Implementation(AActor* HitActor)
{

}

TArray<AActor*> UMonsterSkill::GetHitActors()
{
	return TArray<AActor*>();
}

void UMonsterSkill::OnSkillBegin_Implementation()
{

}

void UMonsterSkill::TickBeforeSkillJudge_Implementation()
{

}

void UMonsterSkill::OnSkillJudgeBegin_Implementation()
{
	APlayerCharacter* Character = UGlobalBlueprintFunctionLibrary::GetPlayerCharacter();
	if (Character)
	{
		IActorInterface::Execute_Hit(Character, OwingMonster, AttackProperty);
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

/**
 * 史莱姆攻击
 */
UMS_SlimeAttack::UMS_SlimeAttack()
{
	AttackProperty = FSkillProperty(EAttackHarmfulType::LightAttack, 1);
}


