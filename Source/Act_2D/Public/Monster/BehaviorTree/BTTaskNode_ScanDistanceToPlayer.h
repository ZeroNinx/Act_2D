// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "MonsterController.h"

//UE4
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ScanDistanceToPlayer.generated.h"

//玩家距离状态
UENUM(BlueprintType)
enum class EDistance :uint8
{
	Far,
	Proper,
	Close
};

/**
 * 检测玩家位置
 */
UCLASS()
class ACT_2D_API UBTTaskNode_ScanDistanceToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	//构造函数
	UBTTaskNode_ScanDistanceToPlayer();

protected:

	//最大近距离
	float MaxClose = 100.0f;

	//最大中距离
	float MaxProper = 500.0f;

	//执行任务
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) override;
};
