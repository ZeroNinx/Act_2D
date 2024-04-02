// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Monster.h"
#include "Monster/MonsterController.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

//构造函数
AMonster::AMonster()
{

	//启用tick
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	//状态机
	StateMachine = CreateDefaultSubobject<UStateMachine>(TEXT("State Machine"));
	StateMachine->SetState(EState::Idle);

	//调整自定义碰撞体
	RealCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RealCapsuleComponent"));
	RealCapsule->SetupAttachment(RootComponent);
	RealCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	RealCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	RealCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

}

bool AMonster::IsAlive()
{
	return GetHealthPoint() > 0;
}

//获取HP
int AMonster::GetHealthPoint()
{
	return HealthPoint;
}

//游戏开始执行
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

//每帧执行
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//更新朝向
void AMonster::UpdateFacingDirection()
{
	APawn* PlayerPawn = UGlobalBlueprintFunctionLibrary::GetPlayerCharacter();
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMonster::UpdateFacingDirection PlayerPawn Invalid"));
		return;
	}

	float PlayerLocationX = PlayerPawn->GetActorLocation().X;
	if (PlayerLocationX < GetActorLocation().X)
	{
		bFacingRight = false;
		GetSprite()->SetRelativeRotation(FRotator(0, 0, 0));
	}
	else
	{
		bFacingRight = true;
		GetSprite()->SetRelativeRotation(FRotator(0, 180.0f, 0));
	}
}

//更新状态
void AMonster::UpdateState()
{
	//获取速度
	FVector Velocy = GetCharacterMovement()->GetLastUpdateVelocity();

	//判断速度
	if (Velocy.Z > 0)
	{
		StateMachine->SetState(EState::Jump);
	}
	else if (Velocy.Z < 0)
	{
		StateMachine->SetState(EState::Fall);
	}
	else
	{
		StateMachine->SetState(EState::Idle);
	}
}

void AMonster::PlayDeathEffect()
{
	SetActorTickEnabled(false);
	GetCapsuleComponent()->SetEnableGravity(false);
	GetSprite()->Stop();

	AMonsterController* MonsterController = Cast<AMonsterController>(GetController());
	MonsterController->StopBehaviorTree();

	DeathFlashCounter = 0;
	auto PlayDeathEffect = [&]()
	{
		bool bVisable = (DeathFlashCounter % 2 == 1);
		this->GetSprite()->SetVisibility(bVisable);
		DeathFlashCounter++;
		if (DeathFlashCounter >= 5)
		{
			GetWorldTimerManager().ClearTimer(DeathEffectTimerHandle);
			this->Destroy();
		}
	};
	GetWorldTimerManager().SetTimer(DeathEffectTimerHandle, PlayDeathEffect, 0.3, true);
}

