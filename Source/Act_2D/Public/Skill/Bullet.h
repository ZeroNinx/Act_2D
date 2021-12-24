// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//自定义
#include "ActorInterface.h"

//UE4
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class ACT_2D_API ABullet : public AActor , public IActorInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	//受击函数
	virtual void Hit_Implementation(AActor* HitActor, FAttackProperty HitAttackProperty) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
