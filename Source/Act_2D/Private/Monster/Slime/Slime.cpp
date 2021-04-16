// Fill out your copyright notice in the Description page of Project Settings.


#include "Slime.h"
#include "SlimeController.h"

//构造函数
ASlime::ASlime()
{

	bFacingRight = false;
	AIControllerClass = ASlimeController::StaticClass();

	//设置变换
	GetCapsuleComponent()->SetCapsuleHalfHeight(34.0f);
	RealCapsule->SetRelativeRotation(FRotator(90.0f,0,0));
	RealCapsule->SetCapsuleHalfHeight(44.0f);
	RealCapsule->SetCapsuleRadius(33.0f);
	GetSprite()->SetRelativeLocation(FVector(0, 0, 44.0f));
	GetArrowComponent()->SetRelativeRotation(FRotator(0, 180.0f, 0));

	//设置摩擦力
	GetCharacterMovement()->AirControl = 0;
	GetCharacterMovement()->GravityScale = 0.8f;
	GetCharacterMovement()->BrakingDecelerationWalking = 200.0f;
	GetCharacterMovement()->GroundFriction = 2.0f;

	//载入动画
	UPaperFlipbook* IdleFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Idle.Slime_Idle'"));
	if(!IdleFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Load Failed");
		return;
	}
	GetSprite()->SetFlipbook(IdleFlipbook);

	//设定受击特效
	UPaperFlipbook* EffectFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Effict.Slime_Effict'"));
	if (!EffectFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Effect Failed"));
		return;
	}
	Effect = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Effect"));
	Effect->SetupAttachment(RootComponent);
	Effect->SetFlipbook(EffectFlipbook);
	Effect->SetLooping(false);

	//设定受击动画
	HitFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Hit.Slime_Hit'"));
	if (!HitFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Hit Flipbook Failed"));
		return;
	}

	//设定攻击组件
	AttackCompnent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("AttackComponent"));
	AttackCompnent->SetupAttachment(GetSprite());
	AttackCompnent->OnComponentHit.AddDynamic(this, &ASlime::OnAttackComponentHit);

	//绑定代理
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &ASlime::OnFlipookFinishedPlaying);
}

//跳跃攻击
void ASlime::JumpAttack()
{
	if (StateMachine->GetState() == EState::Idle&&!bFalled)
	{
		//延迟跳跃
		auto DelayJumpAttack = [&]()->void
		{
			//设定攻击组件
			//FString SpritePath = ;
			UPaperSprite* AttackSprite = LoadObject<UPaperSprite>(this,TEXT("PaperSprite'/Game/Paper2D/Monster/Slime/Jump/Slime_Jump10_Sprite.Slime_Jump10_Sprite'"));
			if (!AttackSprite)
			{
				UKismetSystemLibrary::PrintString(nullptr, FString("Slime Attack Sprite Load Failed"));
				return;
			}
			AttackCompnent->SetSprite(AttackSprite);

			//设定起跳速度
			float DirectMark = bFacingRight ? 1.0f : -1.0f;
			float JumpSpeed = 300.0f * DirectMark;
			GetCharacterMovement()->Velocity = FVector(JumpSpeed, 0, 0);
			Jump();
		};
		auto dlg = FTimerDelegate::CreateLambda(DelayJumpAttack);
		GetWorldTimerManager().SetTimer(JumpAttackHandle, dlg, (const float)JumpReadyTime, false);
	
	}
}

//被击中
void ASlime::Hit(FAttackProperty HitAttackProperty)
{
	AttackProperty = HitAttackProperty;
	PrepareHit();
	Effect->PlayFromStart();

	//添加瞬时速度
	float DirectMark = bFacingRight ? -1.0f : 1.0f;
	float LightVelocyX = 200.0f * DirectMark;
	float HeavyVelocyX = 600.0f * DirectMark;

	if (AttackProperty.HarmfulType == EAttackHarmfulType::HeavyAttack)
	{
		GetCharacterMovement()->Velocity = FVector(HeavyVelocyX, 0, 0);
	}
	else
	{
		GetCharacterMovement()->Velocity = FVector(LightVelocyX, 0, 0);
	}
}

//准备受击
void ASlime::PrepareHit()
{
	//改变状态
	StateMachine->SetState(EState::UnderAttack);

	//初始化受击动画
	GetSprite()->SetLooping(false);
	GetSprite()->SetFlipbook(HitFlipbook);
	GetSprite()->PlayFromStart();
}

//tick函数
void ASlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StateMachine->GetState() != EState::UnderAttack)
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
	if (Velocy.Z > 0)
	{
		StateMachine->SetState(EState::Jumping);
	}
	else if (Velocy.Z < 0)
	{
		StateMachine->SetState(EState::Falling);
	}
	else
	{
		if (StateMachine->GetState() == EState::Falling)
		{
			StateMachine->SetState(EState::Falled);
			bFalled = true;
		}
		else
		{
			StateMachine->SetState(EState::Idle);
		}
	}

}

//更新动画
void ASlime::UpdateAnimation()
{

	//根据不同状态调整动画
	EState CurrentState = StateMachine->GetState();
	if (CurrentState == EState::Falling)
	{
		return;
	}

	//动画路径
	FString AnimationFlipbook;
	switch (CurrentState)
	{
	case EState::Idle:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Idle.Slime_Idle'";
		break;
	case EState::Jumping:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Jump_Start.Slime_Jump_Start'";
		break;
	case EState::Falled:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Jump_End.Slime_Jump_End'";
		break;
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

	bool bShouldLoop = (CurrentState != EState::Jumping && !bFalled);
	GetSprite()->SetLooping(bShouldLoop);
	GetSprite()->SetFlipbook(NewAnimation);
	GetSprite()->Play();
}

//攻击组件
void ASlime::OnAttackComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString Name;
	OtherActor->GetName(Name);
	UKismetSystemLibrary::PrintString(nullptr, FString("Hit ") + Name);
}

//完成受击
void ASlime::OnFlipookFinishedPlaying()
{
	//单帧动画完成时
	bool bShouldUpdate = (StateMachine->GetState() == EState::UnderAttack||bFalled);
	if (bShouldUpdate)
	{
		//清除下落标记
		if (bFalled)
		{
			bFalled = false;
		}
		UpdateState();
		UpdateAnimation();
		GetSprite()->SetLooping(true);
		GetSprite()->Play();
	}
}


