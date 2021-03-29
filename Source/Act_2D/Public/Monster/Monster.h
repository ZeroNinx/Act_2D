// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "StateMachine.h"

//UE4
#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "HAL/PlatformProcess.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Monster.generated.h"


/**
 *	怪物类
 */
UCLASS()
class ACT_2D_API AMonster : public APaperCharacter
{
	GENERATED_BODY()

public:
	//构造函数
	AMonster();
	
	//是否朝右
	bool bFacingRight;

	//获取行为树
	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehaviorTree();

	//被击中函数
	virtual void Hit(FAttackProperty AttackProperty);

protected:

	//碰撞胶囊
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	UCapsuleComponent* RealCapsule;

	//状态机
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	UStateMachine* StateMachine;

	//行为树组件
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBehaviorTreeComponent* BTComponent;

	//游戏开始执行
	virtual void BeginPlay() override;

	//每帧执行
	virtual void Tick(float DeltaTime) override;

};
