// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Slime.h"
#include "SlimeBTComponent.h"

//UE4
#include "CoreMinimal.h"
#include "Monster/MonsterController.h"
#include "SlimeController.generated.h"

/**
 * 史莱姆控制器
 */
UCLASS()
class ACT_2D_API ASlimeController : public AMonsterController
{
	GENERATED_BODY()
	
public:
	
	//构造函数
	ASlimeController();

protected:

	//怪物指针
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ASlime* Slime;

	//当开始控制时
	void OnPossess(APawn* InPawn) override;

};
