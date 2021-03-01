// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackComponent.generated.h"

//��������
USTRUCT(BlueprintType)
struct FAttackAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UPaperFlipbook* Flipbook;//��������

	UPROPERTY(BlueprintReadWrite)
	int ReadyFrames;//ǰҡ֡��

	UPROPERTY(BlueprintReadWrite)
	int AttackFrames;//�����ж�֡��

	UPROPERTY(BlueprintReadWrite)
	int EndFrames;//��ҡ֡��	

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

//�������
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
