#include "Skill/Player/PlayerSkill.h"
#include "Monster/Monster.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

//循环包含
#include "Player/PlayerAttackComponent.h"
#include "Player/PlayerCharacter.h"

//执行攻击
void UPlayerSkill::ExecuteAttackJudge(APlayerCharacter* PlayerCharacter, AActor* HitActor)
{
	IActorInterface::Execute_Hit(HitActor, PlayerCharacter, AttackProperty);

	//打击感延迟
	AMonster* HitMonster = Cast<AMonster>(HitActor);
	if (HitMonster && HitMonster->GetHealthPoint() > 0)
	{
		UGlobalBlueprintFunctionLibrary::SetGlobalDelay(0.05f, 0.07f);
	}
}

void UPlayerSkill::OnAttackJudgeBegin()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacter();
	if (PlayerCharacter)
	{
		for (AActor* HitActor : GetHitActors())
		{
			ExecuteAttackJudge(PlayerCharacter, HitActor);
		}
	}
}

APlayerCharacter* UPlayerSkill::GetPlayerCharacter()
{
	return UGlobalBlueprintFunctionLibrary::GetPlayerCharacter();
}

TArray<AActor*> UPlayerSkill::GetHitActors()
{
	TArray<AActor*> HitActors;
	APlayerCharacter* PlayerCharacter = GetPlayerCharacter();
	if (PlayerCharacter)
	{
		PlayerCharacter->GetAttackComponent()->UpdateOverlaps();
		PlayerCharacter->GetAttackComponent()->GetOverlappingActors(HitActors);
	}
	return MoveTemp(HitActors);
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

void US_AttackIII::OnAttackJudgeBegin()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacter();
	if (PlayerCharacter)
	{
		//添加瞬时速度
		float DirectMark = PlayerCharacter->bFacingRight ? 1.0f : -1.0f;
		float VelocyX = 4000.0f * DirectMark;
		PlayerCharacter->GetCharacterMovement()->Velocity = FVector(VelocyX, 0, 0);
	}
};


void US_AttackIII::TickOnAttackJudge()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacter();
	if (PlayerCharacter)
	{
		for (AActor* HitActor : GetHitActors())
		{
			if (HitActor && !ActorsAppliedHit.Contains(HitActor))
			{
				ExecuteAttackJudge(PlayerCharacter, HitActor);
				ActorsAppliedHit.Add(HitActor);
			}
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

void US_AttackDash::OnAttackJudgeBegin()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacter();
	if (PlayerCharacter)
	{
		//添加瞬时速度
		float DirectMark = PlayerCharacter->bFacingRight ? 1.0f : -1.0f;
		float VelocyX = 10000.0f * DirectMark;
		PlayerCharacter->GetCharacterMovement()->Velocity = FVector(VelocyX, 0, 0);
	}
}

void US_AttackDash::TickOnAttackJudge()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacter();
	if (PlayerCharacter)
	{
		for (AActor* HitActor : GetHitActors())
		{
			if (HitActor && !ActorsAppliedHit.Contains(HitActor))
			{
				ExecuteAttackJudge(PlayerCharacter, HitActor);
				ActorsAppliedHit.Add(HitActor);
			}
		}
	}
}
