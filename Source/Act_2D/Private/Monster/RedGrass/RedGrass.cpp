// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RedGrass/RedGrass.h"
#include "Monster/RedGrass/RedGrassController.h"

//构造函数
ARedGrass::ARedGrass():Super()
{
	bFacingRight = false;
	AIControllerClass = ARedGrassController::StaticClass();

	//设置变换
	GetCapsuleComponent()->SetCapsuleHalfHeight(34.0f);
	RealCapsule->SetRelativeRotation(FRotator(90.0f, 0, 0));
	RealCapsule->SetCapsuleHalfHeight(44.0f);
	RealCapsule->SetCapsuleRadius(33.0f);
	GetSprite()->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));

}
