// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateMachine.generated.h"

UENUM(BlueprintType)
enum class ECharacterState :uint8
{
	Idle,
	Running,
	Jumping,
	Falling,
	Attacking
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UPlayerStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStateMachine();

	//取得状态
	UFUNCTION(BlueprintCallable)
	ECharacterState GetState();

	//设置状态
	UFUNCTION(BlueprintCallable)
	void SetState(ECharacterState NewState);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//取得状态
	UPROPERTY(BlueprintReadWrite)
	ECharacterState State;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
