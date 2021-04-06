// Fill out your copyright notice in the Description page of Project Settings.
#include "Skill.h"

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

void US_AttackIII::InAttack(APlayerCharacter* Player)
{
	//根据速度逐步减少加速度
	FVector Velocy=Player->GetCharacterMovement()->GetLastUpdateVelocity();
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(Velocy.X));

	float DirectMark = Player->bFacingRight ? 1.0f : -1.0f;

	if (!Moved || Velocy.X*DirectMark > 0)//防止角色倒退
	{
		Moved = true;
		Player->AddMovementInput(FVector(1.5f, 0, 0), DirectMark * SpeedScale);
		SpeedScale -= 0.035f;
	}
};


