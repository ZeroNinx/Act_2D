// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerSkill.h"
#include "Kismet/GameplayStatics.h"

//循环包含
#include "PlayerAttackComponent.h"
#include "PlayerCharacter.h"


//执行攻击
void USkill::ExcuteAttackJudge(APlayerCharacter* Player, AMonster* Monster)
{
	Monster->Hit(AttackProperty);

	//打击感延迟
	FPlatformProcess::Sleep(0.07f);
}

void USkill::JudgeAtStartAttack(APlayerCharacter* Player)
{
	ScanHitActors();
	for (AActor* HitActor : HitActors)
	{
		AMonster* HitMonster = Cast<AMonster>(HitActor);
		if(HitMonster)
		{
			ExcuteAttackJudge(Player, HitMonster);
		}
	}
}

void USkill::ScanHitActors()
{
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		PlayerCharacter->GetAttackComponent()->UpdateOverlaps();
		PlayerCharacter->GetAttackComponent()->GetOverlappingActors(HitActors, TSubclassOf<AMonster>());
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
		AMonster* HitMonster = Cast<AMonster>(HitActor);
		if (HitMonster&&!HitMonsters.Contains(HitMonster))
		{
			ExcuteAttackJudge(Player, HitMonster);
			HitMonsters.Add(HitMonster);
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
	for (AActor* HitActor: HitActors)
	{
		AMonster* HitMonster = Cast<AMonster>(HitActor);
		if (HitMonster&&!HitMonsters.Contains(HitMonster))
		{
			ExcuteAttackJudge(Player, HitMonster);
			HitMonsters.Add(HitMonster);
		}
	}
}
