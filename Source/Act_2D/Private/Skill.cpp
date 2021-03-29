// Fill out your copyright notice in the Description page of Project Settings.
#include "Skill.h"

//重复包含
#include "PlayerCharacter.h"

US_AttackIII::US_AttackIII()
{
	
}

void US_AttackIII::InAttack(APlayerCharacter* Player)
{
	//根据速度换算加速度
	FVector velocy=Player->GetCharacterMovement()->GetLastUpdateVelocity();
	UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(velocy.X));
	Player->AddMovementInput(FVector(1.5f, 0, 0), (Player->bFacingRight ? 1.0f : -1.0f)*SpeedScale);
	SpeedScale -= 0.027f;
};
