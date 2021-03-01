// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackComponent.generated.h"

//攻击动作
USTRUCT(BlueprintType)
struct FAttackAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UPaperFlipbook* Flipbook;//攻击动画

	UPROPERTY(BlueprintReadWrite)
	int ReadyFrames;//前摇帧数

	UPROPERTY(BlueprintReadWrite)
	int AttackFrames;//攻击判定帧数

	UPROPERTY(BlueprintReadWrite)
	int EndFrames;//后摇帧数	

	FAttackAction()
	{
		ReadyFrames = 0;
		AttackFrames = 0;
		EndFrames = 0;
	}

	FAttackAction(int Ready, int Attack, int End)
	{
		ReadyFrames = Ready;
		AttackFrames = Attack;
		EndFrames = End;
	}
};

//攻击组件
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UPlayerAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
