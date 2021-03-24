// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "StateMachine.h"

//UE4
#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Monster.generated.h"

UCLASS()
class ACT_2D_API AMonster : public APaperCharacter
{
	GENERATED_BODY()

public:
	//构造函数
	AMonster();

protected:

	//碰撞胶囊
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UCapsuleComponent* RealCapsule;

	//状态机
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UStateMachine* StateMachine;

	//游戏开始执行
	virtual void BeginPlay() override;

	//每帧执行
	virtual void Tick(float DeltaTime) override;

};
