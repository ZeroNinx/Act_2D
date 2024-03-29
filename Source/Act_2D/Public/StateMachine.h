// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Act_2DTypes.h"

//UE4
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"

//基本状态
UENUM(BlueprintType, meta = (Bitflags))
enum class EState : uint8
{
	None		= 0 UMETA(Hidden),
	
	// Lv. 1
	Idle		= 1 << 0	UMETA(DisplayName = "Idle"),
	Run			= 1 << 1	UMETA(DisplayName = "Run"),
	Jump		= 1 << 2	UMETA(DisplayName = "Jump"),
	Fall		= 1 << 3	UMETA(DisplayName = "Fall"),
	Falled		= 1 << 4	UMETA(DisplayName = "Falled"),

	// Lv. 2
	Attack		= 1 << 5	UMETA(DisplayName = "Attack"),

	// Lv.3
	Hit			= 1 << 6	UMETA(DisplayName = "Hit"),
	Sleep		= 1 << 7	UMETA(DisplayName = "Sleep"),
};
ENUM_CLASS_FLAGS(EState);

//基础状态机
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACT_2D_API UStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:
	//构造函数
	UStateMachine();

	//取得状态
	EState GetState();

	//设置状态
	void SetState(EState NewState);

	//判断是否处于状态中
	bool IsInState(EState State);

protected:

	//取得状态
	EState State;

};