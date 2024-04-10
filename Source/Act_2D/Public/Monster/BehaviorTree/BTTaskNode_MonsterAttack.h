// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MonsterAttack.generated.h"

/**
 * 行为树节点：怪物攻击
 */
UCLASS()
class ACT_2D_API UBTTaskNode_MonsterAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	//构造函数
	UBTTaskNode_MonsterAttack();

protected:

	//执行任务
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
