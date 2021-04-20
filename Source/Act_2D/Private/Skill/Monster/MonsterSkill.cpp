#include "MonsterSkill.h"

//循环包含
#include "Monster.h"


void UMonsterSkill::InJudge(AMonster* Monster, APlayerCharacter* Player)
{
	Player->Hit(Monster,AttackProperty);
}


/**
 * 史莱姆攻击
 */
UMS_SlimeAttack::UMS_SlimeAttack()
{
	AttackProperty = FAttackProperty(EAttackHarmfulType::LightAttack, 1);
}


