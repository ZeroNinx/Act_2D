// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BehaviorTree/BTTaskNode_MonsterAttack.h"
#include "AIController.h"
#include "Monster/Monster.h"

UBTTaskNode_MonsterAttack::UBTTaskNode_MonsterAttack():Super()
{

}

//ִ�й���
EBTNodeResult::Type UBTTaskNode_MonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	//��ȡ����
	AMonster* Monster = AIController->GetPawn<AMonster>();
	if (!Monster)
	{
		return EBTNodeResult::Failed;
	}

	Monster->Attack();
	return EBTNodeResult::Succeeded;
}
