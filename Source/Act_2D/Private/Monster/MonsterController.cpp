// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController.h"

//构造函数
AMonsterController::AMonsterController()
{
	
}

//开始控制时
void AMonsterController::OnPossess(APawn* InPawn)
{
	//设置怪物，启动行为树
	Monster = Cast<AMonster>(InPawn);
	RunBehaviorTree(Monster->GetBehaviorTree());
}

