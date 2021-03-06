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
	
	//设置摩擦力
	GetCharacterMovement()->AirControl = 0;
	GetCharacterMovement()->GravityScale = 0.8f;
	GetCharacterMovement()->BrakingDecelerationWalking = 200.0f;
	GetCharacterMovement()->GroundFriction = 2.0f;

	//初始化动画
	InitAnimation();

	//设定受击特效
	HitEffectComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("HitEffectComponent"));
	HitEffectComponent->SetupAttachment(RootComponent);
	HitEffectComponent->SetFlipbook(HitEffectFlipbook);
	HitEffectComponent->SetLooping(false);

	//设定攻击组件
	AttackCompnent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("AttackComponent"));
	AttackCompnent->SetupAttachment(GetSprite());
	AttackCompnent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCompnent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackCompnent->SetVisibility(false);
	AttackCompnent->OnComponentBeginOverlap.AddDynamic(this, &ASlime::OnAttackComponentBeginOverlap);

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

			//检测已存在重叠
			APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			bool bOverlappingPlayer=AttackCompnent->IsOverlappingActor(Player);
			if (bOverlappingPlayer)
			{
				AttackJudge(Player);
			}
		};
		auto dlg = FTimerDelegate::CreateLambda(DelayJumpAttack);
		GetWorldTimerManager().SetTimer(JumpAttackHandle, dlg, (const float)JumpReadyTime, false);
	
	}
}

//被击中
void ASlime::Hit(FAttackProperty HitAttackProperty)
{
	//改变状态
	StateMachine->SetState(EState::Hit);

	//重新播放受击动画
	GetSprite()->SetLooping(false);
	GetSprite()->SetFlipbook(HitFlipbook);
	GetSprite()->PlayFromStart();

	//重新播放效果
	HitEffectComponent->PlayFromStart();

	//添加瞬时速度
	float DirectMark = bFacingRight ? -1.0f : 1.0f;
	float LightVelocyX = 200.0f * DirectMark;
	float HeavyVelocyX = 600.0f * DirectMark;

	if (HitAttackProperty.HarmfulType == EAttackHarmfulType::HeavyAttack)
	{
		GetCharacterMovement()->Velocity = FVector(HeavyVelocyX, 0, 0);
	}
	else
	{
		GetCharacterMovement()->Velocity = FVector(LightVelocyX, 0, 0);
	}
}

//初始化动画资源
void ASlime::InitAnimation()
{
	IdleFlipbook		= LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Idle.Slime_Idle'"));
	JumpingFlipbook		= LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Jump_Start.Slime_Jump_Start'"));
	FalledFlipbook		= LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Jump_End.Slime_Jump_End'"));
	HitFlipbook			= LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Hit.Slime_Hit'"));
	HitEffectFlipbook	= LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Monster/Slime/Slime_Effict.Slime_Effict'"));
	UpdateAnimation();
}

//tick函数
void ASlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StateMachine->GetState() != EState::Hit)
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
		HitEffectComponent->SetRelativeLocation(FVector(-5.0f, 1.0f, 50.0f));
	}
	else
	{
		bFacingRight = true;
		GetSprite()->SetRelativeRotation(FRotator(0, 180.0f, 0));
		HitEffectComponent->SetRelativeLocation(FVector(40.0f, 1.0f, 50.0f));
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
	UPaperFlipbook* AnimationFlipbook=nullptr;
	switch (CurrentState)
	{
	case EState::Idle:
		AnimationFlipbook = IdleFlipbook;
		break;
	case EState::Jumping:
		AnimationFlipbook = JumpingFlipbook;
		break;
	case EState::Falled:
		AnimationFlipbook = FalledFlipbook;
		break;
	}

	if (AnimationFlipbook == nullptr)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString("Slime Flipbook Load Failed"));
		return;
	}

	//设定动画
	bool bShouldLoop = (CurrentState != EState::Jumping && !bFalled);
	GetSprite()->SetLooping(bShouldLoop);
	GetSprite()->SetFlipbook(AnimationFlipbook);
	GetSprite()->Play();
}

//攻击组件重叠时
void ASlime::OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//攻击判定
	bool bShouldJudge = (!bAttacked && (StateMachine->GetState() == EState::Jumping || StateMachine->GetState() == EState::Falling));
	if (bShouldJudge)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (!Player)
		{
			return;
		}
		AttackJudge(Player);
	}
}

//攻击判定
void ASlime::AttackJudge(APlayerCharacter* Player)
{
	//为玩家设定攻击
	bAttacked = true;
	Skill = NewObject<UMS_SlimeAttack>();
	Player->Hit(this, Skill->AttackProperty);
}

//单帧动画播放完成时
void ASlime::OnFlipookFinishedPlaying()
{
	//单帧动画完成时
	bool bShouldUpdate = (StateMachine->GetState() == EState::Hit||bFalled);
	if (bShouldUpdate)
	{
		//下落结束，清除下落标记及攻击标记
		if (bFalled)
		{
			bFalled = false;
			bAttacked = false;
		}

		UpdateState();
		UpdateAnimation();
		GetSprite()->SetLooping(true);
		GetSprite()->Play();
	}
}


