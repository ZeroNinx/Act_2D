// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterController.h"

//UE4
#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameModeBase.h"
#include "Act_2DGameMode.generated.h"

/**
 * 游戏模式
 */
UCLASS()
class ACT_2D_API AAct_2DGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:

	//构造函数
	AAct_2DGameMode();

protected:

	//游戏开始时执行
	void BeginPlay() override;

};
