// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerSkill.h"

//循环包含
#include "PlayerCharacter.h"

//技能类
void USkill::InJudge(APlayerCharacter* Player, AMonster* Monster)
{
	Monster->Hit(AttackProperty);
}


/**
 * AttackI
 */
US_AttackI::US_AttackI()
{
	AttackProperty = FAttackProperty(EAttackHarmfulType::LightAttack, 1);
}

/**
 * AttackII
 */
US_AttackII::US_AttackII()
{
	AttackProperty = FAttackProperty(EAttackHarmfulType::LightAttack, 1);
}



/**
 * AttackIII
 */

US_AttackIII::US_AttackIII()
{
	AttackProperty = FAttackProperty(EAttackHarmfulType::HeavyAttack, 2);
}

void US_AttackIII::BeforeJudge(APlayerCharacter* Player)
{
	//添加瞬时速度
	float DirectMark = Player->bFacingRight ? 1.0f : -1.0f;
	float VelocyX = 900.0f * DirectMark;
	Player->GetCharacterMovement()->Velocity = FVector(VelocyX, 0, 0);
};


/**
 * JumpAttack
 */
US_AttackJump::US_AttackJump()
{
	AttackProperty = FAttackProperty(EAttackHarmfulType::HeavyAttack, 1);
}


/**
 * DashAttack
 */
US_AttackDash::US_AttackDash()
{
	AttackProperty = FAttackProperty(EAttackHarmfulType::LightAttack, 1);
}

void US_AttackDash::BeforeJudge(APlayerCharacter* Player)
{
	//添加瞬时速度
	float DirectMark = Player->bFacingRight ? 1.0f : -1.0f;
	float VelocyX = 2000.0f * DirectMark;
	Player->GetCharacterMovement()->Velocity = FVector(VelocyX, 0, 0);
}