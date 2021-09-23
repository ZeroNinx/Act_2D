// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"

//构造函数
UStateMachine::UStateMachine()
{
	//关闭tick
	PrimaryComponentTick.bCanEverTick = false;
	State = EState::Idle;

}

EState UStateMachine::GetState()
{
	return State;
}

void UStateMachine::SetState(EState NewState)
{
	State = NewState;
}

bool UStateMachine::IsInState(EState InStates)
{
	return ((State & InStates) != EState::None);
}


