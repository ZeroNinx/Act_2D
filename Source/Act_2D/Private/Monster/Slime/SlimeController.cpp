// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeController.h"

//构造函数
ASlimeController::ASlimeController()
{	
	//初始化黑板
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BlackboardComponent->SetValueAsVector(TEXT("PlayerLocation"), FVector(0, 0, 0));
	BlackboardComponent->

	//初始化行为树
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BehaviorTreeComponent->SearchStartBlackboard


}

//当开始控制时
void ASlimeController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Slime = Cast<ASlime>(InPawn);
}
