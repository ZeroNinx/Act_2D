// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "SlimeBTComponent.h"

//UE4
#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Slime.generated.h"

/**
 * 史莱姆
 */
UCLASS()
class ACT_2D_API ASlime : public AMonster
{
	GENERATED_BODY()
	
public:

	//构造函数
	ASlime();

protected:

	//tick函数
	void Tick(float DeltaTime) override;
};
