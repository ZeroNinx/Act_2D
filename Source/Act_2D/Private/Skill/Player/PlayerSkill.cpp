// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerSkill.h"
#include "Kismet/GameplayStatics.h"
#include "Monster.h"

//循环包含
#include "PlayerAttackComponent.h"
#include "PlayerCharacter.h"

//执行攻击
void USkill::ExecuteAttackJudge(APlayerCharacter* Player, AActor* HitActor)
{

	IActorInterface::Execute_Hit(HitActor, Player, AttackProperty);

	//打击感延迟
	if (HitActor->IsA<AMonster>())
	{
		Player->SetGlobalDelay(0.07, 0.1);
	}
}

void USkill::JudgeAtStartAttack(APlayerCharacter* Player)
{
	ScanHitActors();
	for (AActor* HitActor : HitActors)
	{
		ExecuteAttackJudge(Player, HitActor);
	}
}

void USkill::ScanHitActors()
{
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		PlayerCharacter->GetAttackComponent()->UpdateOverlaps();
		PlayerCharacter->GetAttackComponent()->GetOverlappingActors(HitActors);
	}
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

void US_AttackIII::JudgeAtStartAttack(APlayerCharacter* Player)
{
	//添加瞬时速度
	float DirectMark = Player->bFacingRight ? 1.0f : -1.0f;
	float VelocyX = 4000.0f * DirectMark;
	Player->GetCharacterMovement()->Velocity = FVector(VelocyX, 0, 0);
};


void US_AttackIII::JudgeDuringAttack(APlayerCharacter* Player)
{
	ScanHitActors();
	for (AActor* HitActor : HitActors)
	{
		if (HitActor && !ActorsAppliedHit.Contains(HitActor))
		{
			ExecuteAttackJudge(Player, HitActor);
			ActorsAppliedHit.Add(HitActor);
		}
	}
}

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

void US_AttackDash::JudgeAtStartAttack(APlayerCharacter* Player)
{
	//添加瞬时速度
	float DirectMark = Player->bFacingRight ? 1.0f : -1.0f;
	float VelocyX = 10000.0f * DirectMark;
	Player->GetCharacterMovement()->Velocity = FVector(VelocyX, 0, 0);
}

void US_AttackDash::JudgeDuringAttack(APlayerCharacter* Player)
{
	ScanHitActors();
	for (AActor* HitActor : HitActors)
	{
		if (HitActor && !ActorsAppliedHit.Contains(HitActor))
		{
			ExecuteAttackJudge(Player, HitActor);
			ActorsAppliedHit.Add(HitActor);
		}
	}
}
