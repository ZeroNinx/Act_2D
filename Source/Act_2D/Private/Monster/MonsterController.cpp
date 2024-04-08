// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterController.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

//构造函数
AMonsterController::AMonsterController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AMonsterController::StopBehaviorTree()
{
	BehaviorTreeComponent->StopLogic("Monster Dead");
}

void AMonsterController::BeginPlay()
{
	Super::BeginPlay();
}

//开始控制时
void AMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	BehaviorTreeComponent->StartLogic();
}

