// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterController.h"

//构造函数
AMonsterController::AMonsterController()
{
	//载入行为树
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeResource(TEXT("BehaviorTree'/Game/Blueprints/BehaviorTree/Slime/BT_Slime.BT_Slime'"));
	if (!BehaviorTreeResource.Succeeded())
	{
		return;
	}
	BehaviorTree = BehaviorTreeResource.Object;

	//初始化行为树组件
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BehaviorTreeComponent->StartTree(*BehaviorTree);

	//初始化黑板
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

}

//停止行为树
void AMonsterController::StopBehaviorTree()
{
	//BehaviorTreeComponent->PauseLogic("Dead");
}

//开始控制时
void AMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//设置怪物，启动行为树
	Monster = Cast<AMonster>(InPawn);
}

