// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"

//基本状态
UENUM(BlueprintType)
enum class EState :uint8 
{
	Idle,
	Running,
	Jumping,
	Falling,
	Attacking,
	AttackFinished
};

//基础状态机
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACT_2D_API UStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:
	//构造函数
	UStateMachine();

	//取得状态
	UFUNCTION(BlueprintCallable)
	EState GetState();

	//设置状态
	UFUNCTION(BlueprintCallable)
	void SetState(EState NewState);

protected:

	//取得状态
	UPROPERTY(BlueprintReadWrite)
	EState State;

};