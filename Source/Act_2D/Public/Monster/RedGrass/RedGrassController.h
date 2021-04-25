// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "RedGrass.h"

//UE4
#include "CoreMinimal.h"
#include "AIController.h"
#include "RedGrassController.generated.h"

/**
 * 红草控制器
 */
UCLASS()
class ACT_2D_API ARedGrassController : public AAIController
{
	GENERATED_BODY()
	
public:

	//构造函数
	ARedGrassController();

protected:

	//怪物指针
	UPROPERTY()
	ARedGrass* RedGrass;

	//当开始控制时
	void OnPossess(APawn* InPawn) override;

};
