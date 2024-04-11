// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "Skill/Monster/MonsterSkill.h"
#include "MonsterSkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillJudgeBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillJudgeEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEnd);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UMonsterSkillComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

public:	

	//���캯��
	UMonsterSkillComponent();

	//�ͷż���
	UFUNCTION(BlueprintCallable)
	void UseSkill(UMonsterSkill* NewSkill);

	//���ü���
	void ResetSkill();

	//���ü����ж���ʼ
	UFUNCTION(BlueprintCallable)
	void SetSkillJudgeBegin();

	//���ü����жϽ���
	UFUNCTION(BlueprintCallable)
	void SetSkillJudgeEnd();

	//���ü��ܽ����������Լ��ܲ���Ӱ�죩
	UFUNCTION(BlueprintCallable)
	void SetSkillEnd();

	//ǿ�Ƽ��ܽ���
	UFUNCTION(BlueprintCallable)
	void ForceSkillEnd();

public:

	// ���ܿ�ʼ�¼�
	UPROPERTY(BlueprintAssignable)
	FOnSkillBegin OnSkillBegin;

	// �����ж���ʼ�¼�
	UPROPERTY(BlueprintAssignable)
	FOnSkillJudgeBegin OnSkillJudgeBegin;

	// �����ж������¼�
	UPROPERTY(BlueprintAssignable)
	FOnSkillJudgeEnd OnSkillJudgeEnd;

	// ���ܽ����¼�
	UPROPERTY(BlueprintAssignable)
	FOnSkillEnd OnSkillEnd;

protected:

	//���ܿ�ʼʱ
	void SkillBegin();

	//�����ж���ʼʱ
	void SkillJudgeBegin();

	//�����ж�����ʱ
	void SkillJudgeEnd();

	//���ܽ���ʱ
	void SkillEnd();

protected:

	class AMonster* OwingMonster;

	//�����ж��Ƿ��ѿ�ʼ
	bool bSkillJudgeBegin = false;

	//�����ж��Ƿ��ѽ���
	bool bSkillJudgeEnd = false;

	UPROPERTY()
	UMonsterSkill* CurrentSkill;

	//�����б�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UMonsterSkill*> SkillList;

	//��ʼ������
	void SetupSkill();

	//Tick����
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
