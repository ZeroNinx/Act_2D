// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RedGrass/RedGrassController.h"

//构造函数
ARedGrassController::ARedGrassController()
{

}

//当开始控制时
void ARedGrassController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RedGrass = Cast<ARedGrass>(InPawn);
}
