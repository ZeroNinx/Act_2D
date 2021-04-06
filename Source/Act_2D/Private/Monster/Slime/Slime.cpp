// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime.h"

//构造函数
ASlime::ASlime():AMonster()
{
	bFacingRight = false;



	//设置变换
	GetCapsuleComponent()->SetCapsuleHalfHeight(34.0f);
	RealCapsule->SetRelativeRotation(FRotator(90.0f,0,0));
	RealCapsule->SetCapsuleHalfHeight(44.0f);
	RealCapsule->SetCapsuleRadius(33.0f);
	GetSprite()->SetRelativeLocation(FVector(0, 0, 44.0f));

	//载入动画
	UPaperFlipbook* IdleFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Idle.Slime_Idle'"));
	if(!IdleFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Load Failed");
		return;
	}
	GetSprite()->SetFlipbook(IdleFlipbook);

	//设定特效
	UPaperFlipbook* EffectFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Effict.Slime_Effict'"));
	if (!EffectFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Effect Failed"));
		return;
	}
	Effect = CreateDefaultSubobject<UPaperFlipbookComponent>("Effect");
	Effect->SetupAttachment(RootComponent);
	Effect->SetFlipbook(EffectFlipbook);
	Effect->SetLooping(false);
	Effect->OnFinishedPlaying.Add(OnEffectPlayFinishedDelegate);

	//设定行为树
	BTComponent = CreateDefaultSubobject<USlimeBTComponent>(TEXT("BTComponent"));


	OnEffectPlayFinishedDelegate.BindDynamic(this, &ASlime::RemoveEffect);
	OnDamagedDelegate.BindDynamic(this, &ASlime::FinishDamaged);
}

//被击中
void ASlime::Hit(FAttackProperty AttackProperty)
{
	PrepareHit();
	Effect->PlayFromStart();
}

//准备受击
void ASlime::PrepareHit()
{
	//准备下一次攻击后退
	bMovedOnHit = false;
	SpeedScale = 1.0f;

	//改变状态
	StateMachine->SetState(EState::UnderAttack);

	//初始化受击动画
	UPaperFlipbook* HitFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Hit.Slime_Hit'"));
	if (!HitFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Hit Flipbook Failed"));
		return;
	}

	if (GetSprite()->GetFlipbook() == HitFlipbook)
	{
		GetSprite()->PlayFromStart();
	}
	GetSprite()->SetFlipbook(HitFlipbook);
	GetSprite()->SetLooping(false);
	GetSprite()->OnFinishedPlaying.Add(OnDamagedDelegate);
}

//tick函数
void ASlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StateMachine->GetState() == EState::UnderAttack)
	{
		MoveBack();
	}
	else
	{
		UpdateDirection();
		UpdateState();
		UpdateAnimation();
	}

}

//更新方向
void ASlime::UpdateDirection()
{
	float PlayerLocationX = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().X;
	if (PlayerLocationX < GetActorLocation().X)
	{
		bFacingRight = false;
		GetSprite()->SetRelativeRotation(FRotator(0, 0, 0));
		Effect->SetRelativeLocation(FVector(-5.0f, 1.0f, 50.0f));
	}
	else
	{
		bFacingRight = true;
		GetSprite()->SetRelativeRotation(FRotator(0, 180.0f, 0));
		Effect->SetRelativeLocation(FVector(40.0f, 1.0f, 50.0f));
	}
}

//更新状态
void ASlime::UpdateState()
{
	//获取速度
	FVector Velocy = GetCharacterMovement()->GetLastUpdateVelocity();

	//判断速度
	if (UKismetMathLibrary::Abs(Velocy.Z) > 0)
	{
		StateMachine->SetState(EState::Jumping);
	}
	else
	{
		StateMachine->SetState(EState::Idle);
	}
}

//更新动画
void ASlime::UpdateAnimation()
{
	//动画路径
	FString AnimationFlipbook;

	//根据不同状态调整动画
	switch (StateMachine->GetState())
	{
	case EState::Idle:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Idle.Slime_Idle'";
		break;
	case EState::Jumping:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Jump.Slime_Jump'";
		break;;
	case EState::Sleeping:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Sleep.Slime_Sleep'";
		break;
	}

	//设定动画
	UPaperFlipbook* NewAnimation = LoadObject<UPaperFlipbook>(GetWorld(), *AnimationFlipbook);
	if (!NewAnimation)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Slime Animation Flipbook Failed"));
		return;
	}

	GetSprite()->SetFlipbook(NewAnimation);
}

//移除特效
void ASlime::RemoveEffect()
{
	StateMachine->SetState(EState::Idle);
}

//后退
void ASlime::MoveBack()
{
	//根据速度逐步减少加速度
	FVector Velocy = GetCharacterMovement()->GetLastUpdateVelocity();
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(Velocy.X));

	float DirectMark = bFacingRight ? -1.0f : 1.0f;

	if (!bMovedOnHit || Velocy.X * DirectMark > 0)//防止角色倒退
	{
		bMovedOnHit = true;
		AddMovementInput(FVector(1.5f, 0, 0), DirectMark * SpeedScale);
		SpeedScale -= 0.1f;
	}
}

//完成受击
void ASlime::FinishDamaged()
{
	UpdateState();
	GetSprite()->SetLooping(true);
	GetSprite()->Play();
}

