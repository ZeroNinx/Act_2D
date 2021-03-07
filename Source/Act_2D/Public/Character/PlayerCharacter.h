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
	// ���캯��
	APlayerCharacter();

protected:


	// ��Ϸ��ʼִ��
	virtual void BeginPlay() override;

	//��ɫ����
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPaperFlipbookComponent* Flipbook;

	//״̬��
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPlayerStateMachine* StateMachine;

	//�������
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPlayerAttackComponent* AttackComponent;


public:	
	//Tick����
	virtual void Tick(float DeltaTime) override;

	//�����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
