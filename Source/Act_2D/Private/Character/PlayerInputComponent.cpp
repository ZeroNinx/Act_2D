// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputComponent.h"

//构造函数
UPlayerInputComponent::UPlayerInputComponent()
{

}

//设定持有者
void UPlayerInputComponent::SetOwner(APlayerCharacter* Owner)
{
	Player = Owner;
}
