// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "PlayerInputComponent.generated.h"

class APlayerCharacter;

/**
 * 玩家输入组件
 */

UCLASS()
class ACT_2D_API UPlayerInputComponent : public UInputComponent
{
	GENERATED_BODY()

public:

	//构造函数
	UPlayerInputComponent();

	//设定持有者
	UFUNCTION(BlueprintCallable)
	void SetOwner(APlayerCharacter* Owner);

private:

	//持有的玩家
	UPROPERTY()
	APlayerCharacter* Player;
};
