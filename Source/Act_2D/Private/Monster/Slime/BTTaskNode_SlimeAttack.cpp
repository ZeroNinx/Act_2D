// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Slime/BTTaskNode_SlimeAttack.h"

//构造函数
UBTTaskNode_SlimeAttack::UBTTaskNode_SlimeAttack()
{

}

//执行攻击
EBTNodeResult::Type UBTTaskNode_SlimeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	//获取怪物
	ASlime* Slime = AIController->GetPawn<ASlime>();
	if (!Slime)
	{
		return EBTNodeResult::Failed;
	}

	Slime->JumpAttack();

	return EBTNodeResult::Succeeded;
}
