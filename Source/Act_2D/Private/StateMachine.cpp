// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"

//构造函数
UStateMachine::UStateMachine()
{
	//关闭tick
	PrimaryComponentTick.bCanEverTick = false;
	State = EState::Idle;

}


//取得状态
EState UStateMachine::GetState()
{
	return State;
}

//设置状态
void UStateMachine::SetState(EState NewState)
{
	State = NewState;
}

