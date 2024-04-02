// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Slime/BTTaskNode_SlimeAttack.h"

//构造函数
UBTTaskNode_SlimeAttack::UBTTaskNode_SlimeAttack()
{

}

//执行攻击
EBTNodeResult::Type UBTTaskNode_SlimeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//获取怪物
	ASlime* Slime = Cast<ASlime>(OwnerComp.GetAIOwner()->GetPawn());
	Slime->JumpAttack();
	
	return EBTNodeResult::Succeeded;
}
