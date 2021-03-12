// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStateMachine.h"

// Sets default values for this component's properties
UPlayerStateMachine::UPlayerStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	State = CharacterState::Idle;
	// ...
}


// Called when the game starts
void UPlayerStateMachine::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//取得状态
CharacterState UPlayerStateMachine::GetState()
{
	return State;
}

//设置状态
void UPlayerStateMachine::SetState(CharacterState NewState)
{
	State = NewState;
}

// Called every frame
void UPlayerStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

