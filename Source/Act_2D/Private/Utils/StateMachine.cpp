// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/StateMachine.h"

//构造函数
UStateMachine::UStateMachine()
{
	//关闭tick
	PrimaryComponentTick.bCanEverTick = false;

}

EState UStateMachine::GetState()
{
	return CurrentState;
}

void UStateMachine::SetState(EState NewState)
{
	if (NewState != CurrentState)
	{
		OnExitState.Broadcast(CurrentState);
		CurrentState = NewState;
		OnEnterState.Broadcast(CurrentState);
	}

}



