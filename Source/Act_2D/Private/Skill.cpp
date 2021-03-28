// Fill out your copyright notice in the Description page of Project Settings.
#include "Skill.h"

//重复包含
#include "PlayerCharacter.h"

void US_AttackIII::BeforeAttack(APlayerCharacter* Player)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString("aaa"));
};