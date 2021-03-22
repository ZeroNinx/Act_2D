// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateMachine.h"

//构造函数
UPlayerStateMachine::UPlayerStateMachine()
{
	//初始化状态
	PrimaryComponentTick.bCanEverTick = false;
	State = ECharacterState::Idle;
	// ...
}


//游戏开始时执行
void UPlayerStateMachine::BeginPlay()
{
	Super::BeginPlay();
	
}

//取得状态
ECharacterState UPlayerStateMachine::GetState()
{
	return State;
}

//设置状态
void UPlayerStateMachine::SetState(ECharacterState NewState)
{
	State = NewState;
}

//每帧执行
void UPlayerStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

