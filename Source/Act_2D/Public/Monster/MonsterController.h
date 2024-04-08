// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "Monster.h"

//UE4
#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "MonsterController.generated.h"

/**
 * 怪物控制器
 */
UCLASS()
class ACT_2D_API AMonsterController : public AAIController
{
	GENERATED_BODY()

public:

	//构造函数
	AMonsterController();

	//玩家距离
	UPROPERTY(BlueprintReadOnly)
	FName DistanceToPlayerID = TEXT("DistanceToPlayer");

	UFUNCTION(BlueprintCallable)
	void StopBehaviorTree();

protected:

	//行为树组件
	UPROPERTY(EditDefaultsOnly);
	UBehaviorTreeComponent* BehaviorTreeComponent;

	//黑板组件
	UPROPERTY(EditDefaultsOnly)
	UBlackboardComponent* BlackboardComponent;

	virtual void BeginPlay() override;

	//控制时
	virtual void OnPossess(APawn* InPawn) override;
};
