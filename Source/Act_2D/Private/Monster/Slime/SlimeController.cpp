// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeController.h"

//构造函数
ASlimeController::ASlimeController()
{

}

//当开始控制时
void ASlimeController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Slime = Cast<ASlime>(InPawn);
}
