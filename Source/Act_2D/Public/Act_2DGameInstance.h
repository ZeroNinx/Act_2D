// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Player/PlayerCharacter.h"
#include "Blueprint/UserWidget.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Act_2DGameInstance.generated.h"

/**
 * 游戏实例类
 */
UCLASS()
class ACT_2D_API UAct_2DGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UAct_2DGameInstance();

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	UUserWidget* MainUI;

};
