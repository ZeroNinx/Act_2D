// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//UE4

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UCLASS()
class ACT_2D_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	//构造函数
	AMonster();

protected:

	//动画组件
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UPaperFlipbookComponent* FlipbookComponent;

	//游戏开始执行
	virtual void BeginPlay() override;

	//每帧执行
	virtual void Tick(float DeltaTime) override;

};
