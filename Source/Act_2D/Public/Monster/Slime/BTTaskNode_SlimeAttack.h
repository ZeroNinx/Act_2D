// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slime.h"
#include "AIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SlimeAttack.generated.h"

/**
 * 史莱姆攻击
 */
UCLASS()
class ACT_2D_API UBTTaskNode_SlimeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	
	//构造函数
	UBTTaskNode_SlimeAttack();

protected:

	//执行任务
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
