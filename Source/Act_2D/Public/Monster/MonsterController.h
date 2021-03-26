// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Monster.h"

//UE4
#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MonsterController.generated.h"

/**
 * 怪物控制器
 */
UCLASS()
class ACT_2D_API AMonsterController : public AAIController
{
	GENERATED_BODY()

public:

	//构造函数
	AMonsterController();

protected:

	//怪物指针
	UPROPERTY(BlueprintReadOnly)
	AMonster* Monster;

	//开始控制时
	virtual void OnPossess(APawn* InPawn) override;
};
