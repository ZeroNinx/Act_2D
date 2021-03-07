// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateMachine.generated.h"

UENUM(BlueprintType)
enum class CharacterState :uint8
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

	//ȡ��״̬
	UFUNCTION(BlueprintCallable)
	CharacterState GetState();

	//����״̬
	UFUNCTION(BlueprintCallable)
	void SetState(CharacterState NewState);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//ȡ��״̬
	UPROPERTY(BlueprintReadWrite)
	CharacterState State;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
