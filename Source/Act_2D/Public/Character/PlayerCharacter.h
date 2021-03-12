// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Character/PlayerStateMachine.h"
#include "Character/PlayerAttackComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ACT_2D_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// 构造函数
	APlayerCharacter();

protected:


	// 游戏开始执行
	virtual void BeginPlay() override;

	//角色动画
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPaperFlipbookComponent* Flipbook;

	//状态机
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPlayerStateMachine* StateMachine;

	//攻击组件
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPlayerAttackComponent* AttackComponent;


public:	
	//Tick函数
	virtual void Tick(float DeltaTime) override;

	//输入绑定
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
