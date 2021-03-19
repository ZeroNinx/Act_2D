// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

//构造函数
APlayerCharacter::APlayerCharacter()
{
 	//启用Tick
	PrimaryActorTick.bCanEverTick = true;

	//默认角色朝右
	bFacingRight = true;
	
	//角色动画
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipBook"));
	FlipbookComponent->SetupAttachment(RootComponent);
	FlipbookComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	FlipbookComponent->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

	//状态机
	StateMachine = CreateDefaultSubobject<UPlayerStateMachine>(TEXT("StateMachine"));

	//攻击组件
	AttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("AttackComponent"));
	AttackComponent->SetupAttachment(FlipbookComponent);
	AttackComponent->Setup(FlipbookComponent, StateMachine);

	//绑定代理到动画播放结束
	OnAttackFinishedDelegate.BindDynamic(this,&APlayerCharacter::AttackRestore);
	FlipbookComponent->OnFinishedPlaying.Add(OnAttackFinishedDelegate);
}

//开始游戏
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}



//Tick函数
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(FlipbookComponent->IsLooping()));

	//当非战斗时自动调整动画
	if (StateMachine->GetState() != ECharacterState::Attacking)
	{
		UpdateDirection();
		UpdateState();
		UpdateAnimation();
	}
}

//调整方向
void APlayerCharacter::UpdateDirection()
{
	if (bFacingRight)
	{
		FlipbookComponent->SetRelativeRotation(FRotator(0, 0, 0));
	}
	else
	{
		FlipbookComponent->SetRelativeRotation(FRotator(0, 180.0f, 0));
	}
}

//调整状态
void APlayerCharacter::UpdateState()
{
	//获取速度
	FVector Velocy = GetCharacterMovement()->GetLastUpdateVelocity();

	//根据Z轴速度判断起跳/下落
	if (Velocy.Z> 0)
	{
		StateMachine->SetState(ECharacterState::Jumping);
	}
	else if(Velocy.Z <0)
	{
		StateMachine->SetState(ECharacterState::Falling);
	}
	else
	{
		//否则根据X轴速度判断奔跑/静止
		if (abs(Velocy.X) != 0)
		{
			StateMachine->SetState(ECharacterState::Running);
			
		}
		else
		{
			StateMachine->SetState(ECharacterState::Idle);
		}
		
	}
}

//调整动画
void APlayerCharacter::UpdateAnimation()
{
	//动画路径
	FString AnimationFlipbook;
	
	//根据不同状态调整动画
	switch (StateMachine->GetState())
	{
	case ECharacterState::Idle:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Character/PlayerCharacter.PlayerCharacter'";
		break;
	case ECharacterState::Running:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Character/Run/Character_Run.Character_Run'";
		break;
	case ECharacterState::Jumping:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Character/Jump/Character_Jump_Start.Character_Jump_Start'";
		break;;
	case ECharacterState::Falling:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Character/Jump/Character_Jump_Fall.Character_Jump_Fall'";
		break;
	}

	//设定动画
	UPaperFlipbook* NewAnimation = LoadObject<UPaperFlipbook>(GetWorld(),*AnimationFlipbook);
	if (!NewAnimation)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Animation Flipbook Failed"));
		return;
	}

	FlipbookComponent->SetFlipbook(NewAnimation);
}



//按键绑定
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//绑定轴输入
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &APlayerCharacter::MoveUp);
	
	//攻击键
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::AttackPresssed);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacter::AttackReleased);

	//特殊键
	PlayerInputComponent->BindAction("Special", IE_Pressed, this, &APlayerCharacter::SpecialPresssed);
	PlayerInputComponent->BindAction("Special", IE_Released, this, &APlayerCharacter::SpecialReleased);

	//扳机键
	PlayerInputComponent->BindAction("Trigger", IE_Pressed, this, &APlayerCharacter::TriggerPresssed);
	PlayerInputComponent->BindAction("Trigger", IE_Released, this, &APlayerCharacter::TriggerReleased);

	//跳跃键
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::JumpReleased);

}

//角色移动
void APlayerCharacter::MoveRight(float AxisValue)
{
	//当输入操作时
	if (AxisValue != 0)
	{
		AttackComponent->bRightPressed = (fabs(AxisValue - 1.0f) <= eps);
		AttackComponent->bLeftPressed = (fabs(AxisValue + 1.0f) <= eps);

		//根据角色方向调整动画
		bFacingRight = AxisValue > 0;

		//添加移动
		AddMovementInput(FVector(1,0,0), AxisValue);
	}
}

void APlayerCharacter::MoveUp(float AxisValue)
{
	//当输入操作时
	if (AxisValue != 0)
	{
		AttackComponent->bUpPressed = (fabs(AxisValue - 1.0f) <= eps);
		AttackComponent->bDownPressed = (fabs(AxisValue + 1.0f) <= eps);

	}
}

//按下攻击键
void APlayerCharacter::AttackPresssed()
{
	AttackComponent->bAttackPressed = true;

	//延迟攻击输入
	AddAttackInput();
}

//松开攻击键
void APlayerCharacter::AttackReleased()
{
	AttackComponent->bAttackPressed = false;
}

//按下特殊键
void APlayerCharacter::SpecialPresssed()
{
	AttackComponent->bSpecialPressed = true;
	//延迟攻击输入
	AddAttackInput();
}

//松开特殊键
void APlayerCharacter::SpecialReleased()
{
	AttackComponent->bSpecialPressed = false;
}

//按下扳机键
void APlayerCharacter::TriggerPresssed()
{
	AttackComponent->bTriggerPressed = true;
	//添加攻击输入
	AddAttackInput();
}

//松开扳机键
void APlayerCharacter::TriggerReleased()
{
	AttackComponent->bTriggerPressed = false;
}

//按下跳跃键
void APlayerCharacter::JumpPressed()
{
	AttackComponent->bJumpPressed = true;
	AttackComponent->RecordKeyCombination();

	Jump();
}

//松开跳跃键
void APlayerCharacter::JumpReleased()
{
	AttackComponent->bJumpPressed = false;
}

//添加攻击输入
void APlayerCharacter::AddAttackInput()
{
	//非攻击是进入攻击状态
	if (GetState() != ECharacterState::Attacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
		Attack();
	}
	else if(AttackComponent->IsAcceptInput())
	{
		//设置延迟接受输入
		auto DelayAttackInput = [&]() -> void
		{
			AttackComponent->RecordKeyCombination();
		};

		auto dlg = FTimerDelegate::CreateLambda(DelayAttackInput);
		GetWorldTimerManager().SetTimer(AttackDelayHandle, dlg, (const float)AttackInputDuration, false);
	}
}


//取得状态
ECharacterState APlayerCharacter::GetState()
{
	return StateMachine->GetState();
}

//取得动画组件
UPaperFlipbookComponent* APlayerCharacter::GetFlipbookComponent()
{
	return FlipbookComponent;
}

//取得状态机
UPlayerStateMachine* APlayerCharacter::GetStateMachine()
{
	return StateMachine;
}

//攻击
void APlayerCharacter::Attack()
{
	AttackComponent->Attack();
}

//从攻击恢复
void APlayerCharacter::AttackRestore()
{
	AttackComponent->ResetAttack();
	StateMachine->SetState(ECharacterState::Idle);
	FlipbookComponent->SetLooping(true);
	FlipbookComponent->Play();
}
