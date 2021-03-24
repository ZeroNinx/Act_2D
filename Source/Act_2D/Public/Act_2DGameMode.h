// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"

//UE4
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerStart.h"
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

	//玩家重生地
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	APlayerStart* PlayerStart;

	//玩家重生地变换
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	FTransform PlayerStartTransform;

	//玩家指针
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	APlayerCharacter* PlayerCharacter;

	//游戏开始时执行
	void BeginPlay() override;

	//重置玩家
	UFUNCTION(BlueprintCallable)
	void ResetPlayer();

};
