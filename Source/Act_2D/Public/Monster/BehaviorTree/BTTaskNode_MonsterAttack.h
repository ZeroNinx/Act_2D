// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MonsterAttack.generated.h"

/**
 * ��Ϊ���ڵ㣺���﹥��
 */
UCLASS()
class ACT_2D_API UBTTaskNode_MonsterAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	//���캯��
	UBTTaskNode_MonsterAttack();

protected:

	//ִ������
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
