// Fill out your copyright notice in the Description page of Project Settings.


#include "RedGrass.h"
#include "RedGrassController.h"

//构造函数
ARedGrass::ARedGrass()
{
	bFacingRight = false;
	AIControllerClass = ARedGrassController::StaticClass();

	//设置变换
	GetCapsuleComponent()->SetCapsuleHalfHeight(34.0f);
	RealCapsule->SetRelativeRotation(FRotator(90.0f, 0, 0));
	RealCapsule->SetCapsuleHalfHeight(44.0f);
	RealCapsule->SetCapsuleRadius(33.0f);
	GetSprite()->SetRelativeLocation(FVector(0, 0, 28.0f));
	GetSprite()->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));
	
	//更新动画
	InitAnimation();

	//设定受击特效
	HitEffectComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("HitEffectComponent"));
	HitEffectComponent->SetupAttachment(RootComponent);
	HitEffectComponent->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));
	HitEffectComponent->SetFlipbook(HitEffectFlipbook);
	HitEffectComponent->SetLooping(false);

	//绑定代理
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &ARedGrass::OnFlipookFinishedPlaying);
}

//攻击
void ARedGrass::Attack()
{
	//改变状态
	StateMachine->SetState(EState::Hit);

	//重新播放受击动画
	GetSprite()->SetLooping(false);
	GetSprite()->SetFlipbook(HitFlipbook);
	GetSprite()->PlayFromStart();

	//重新播放效果
	HitEffectComponent->PlayFromStart();
}

//受击函数
void ARedGrass::Hit_Implementation(AActor* HitActor, FAttackProperty HitAttackProperty)
{
	
	//改变状态
	StateMachine->SetState(EState::Hit);

	//重新播放受击动画
	GetSprite()->SetLooping(false);
	GetSprite()->SetFlipbook(HitFlipbook);
	GetSprite()->PlayFromStart();

	//重新播放效果
	HitEffectComponent->PlayFromStart();

	if (HitAttackProperty.HarmfulType == EAttackHarmfulType::HeavyAttack)
	{
		HealthPoint -= 2;
	}
	else
	{
		HealthPoint -= 1;
	}

	//死亡判断
	if (HealthPoint <= 0)
	{
		PlayDeathEffect();
	}
}

//每帧执行
void ARedGrass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StateMachine->GetState() != EState::Hit)
	{
		UpdateFacingDirection();
		UpdateState();
		UpdateAnimation();
	}
}

//初始化动画
void ARedGrass::InitAnimation()
{
	IdleFlipbook		= LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Monster/RedGrass/Red_Grass.Red_Grass'"));
	HitFlipbook			= LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Monster/RedGrass/Hit.Hit'"));
	HitEffectFlipbook	= LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Monster/RedGrass/Effect_Hit.Effect_Hit'"));
	UpdateAnimation();
}

//更新动画
void ARedGrass::UpdateAnimation()
{
	//根据不同状态调整动画
	EState CurrentState = StateMachine->GetState();
	if (CurrentState == EState::Falling)
	{
		return;
	}

	//动画路径
	UPaperFlipbook* AnimationFlipbook = nullptr;
	switch (CurrentState)
	{
	case EState::Idle:
		AnimationFlipbook = IdleFlipbook;
		break;
	}

	if (AnimationFlipbook == nullptr)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString("RedGrass Flipbook Load Failed"));
		return;
	}

	//设定动画
	GetSprite()->SetLooping(true);
	GetSprite()->SetFlipbook(AnimationFlipbook);
	GetSprite()->Play();
}

//单帧动画播放完成时
void ARedGrass::OnFlipookFinishedPlaying()
{
	//单帧动画完成时
	bool bShouldUpdate = (StateMachine->GetState() == EState::Hit);
	if (bShouldUpdate)
	{
		//下落结束，清除下落标记及攻击标记
		bAttacked = false;
		
		UpdateState();
		UpdateAnimation();
		GetSprite()->SetLooping(true);
		GetSprite()->Play();
	}
}
