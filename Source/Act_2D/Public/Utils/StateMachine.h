// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Utils/Types.h"

//UE4
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"

//基本状态
UENUM(BlueprintType)
enum class EState : uint8
{
	None		= 0 UMETA(Hidden),
	Idle		= 1	UMETA(DisplayName = "Idle"),
	Run			= 2 UMETA(DisplayName = "Run"),
	Jump		= 3 UMETA(DisplayName = "Jump"),
	Fall		= 4 UMETA(DisplayName = "Fall"),
	Attack		= 5 UMETA(DisplayName = "Attack"),
	Hit			= 6 UMETA(DisplayName = "Hit"),
	Sleep		= 7 UMETA(DisplayName = "Sleep"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnterState, EState, CurrentState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExitState, EState, CurrentState);

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

	//进入状态事件
	UPROPERTY(BlueprintAssignable)
	FOnEnterState OnEnterState;

	//退出状态事件
	UPROPERTY(BlueprintAssignable)
	FOnExitState OnExitState;

protected:

	//当前状态
	EState CurrentState = EState::None;

};