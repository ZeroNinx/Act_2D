// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义


//UE4
#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Components/ArrowComponent.h"
#include "RedGrass.generated.h"

/**
 * 红草
 */
UCLASS()
class ACT_2D_API ARedGrass : public AMonster
{
	GENERATED_BODY()
	
public:

	//构造函数
	ARedGrass();

};
