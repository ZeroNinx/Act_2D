// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

//构造函数
APlayerCharacter::APlayerCharacter()
{
 	//启用Tick
	PrimaryActorTick.bCanEverTick = true;

	//默认角色朝右
	bFacingRight = true;

	//使用自定义输入组件
	MyPlayerInputComponent = CreateDefaultSubobject<UPlayerInputComponent>(TEXT("PlayerInputComponent"));
	SetupPlayerInputComponent(MyPlayerInputComponent);

	//角色动画
	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipBook"));
	Flipbook->SetupAttachment(RootComponent);
	Flipbook->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	Flipbook->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

	//状态机
	StateMachine = CreateDefaultSubobject<UPlayerStateMachine>(TEXT("StateMachine"));

	//攻击组件
	AttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("AttackComponent"));
	AttackComponent->SetupAttachment(Flipbook);
	AttackComponent->SetFlipbook(Flipbook);

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

	//当非战斗时自动调整动画
	if (StateMachine->GetState() != CharacterState::Attacking)
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
		Flipbook->SetRelativeRotation(FRotator(0, 0, 0));
	}
	else
	{
		Flipbook->SetRelativeRotation(FRotator(0, 180.0f, 0));
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
		StateMachine->SetState(CharacterState::Jumping);
	}
	else if(Velocy.Z <0)
	{
		StateMachine->SetState(CharacterState::Falling);
	}
	else
	{
		//否则根据X轴速度判断奔跑/静止
		if (abs(Velocy.X) != 0)
		{
			StateMachine->SetState(CharacterState::Running);
		}
		else
		{
			StateMachine->SetState(CharacterState::Idle);
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
	case CharacterState::Idle:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Character/PlayerCharacter.PlayerCharacter'";
		break;
	case CharacterState::Running:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Character/Run/Character_Run.Character_Run'";
		break;
	case CharacterState::Jumping:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Character/Jump/Character_Jump_Start.Character_Jump_Start'";
		break;;
	case CharacterState::Falling:
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

	Flipbook->SetFlipbook(NewAnimation);
}


//按键绑定
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//绑定输入
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	//按下攻击
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::AttackPresssed);

	//按下跳跃
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpPressed);

}

//角色移动
void APlayerCharacter::MoveRight(float AxisValue)
{
	//当输入操作时
	if (AxisValue != 0)
	{
		//根据角色方向调整动画
		bFacingRight = AxisValue > 0;

		//添加移动
		AddMovementInput(FVector(1,0,0), AxisValue);

	}
}

//按下攻击键
void APlayerCharacter::AttackPresssed()
{

	//改变状态为攻击
	StateMachine->SetState(CharacterState::Attacking);
	Flipbook->SetLooping(false);
	AttackComponent->Attack();

	//设置延迟执行攻击结束
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "AttackRestore";
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = 0;
	UKismetSystemLibrary::Delay(this,Flipbook->GetFlipbookLength(),LatentInfo);


}

//攻击恢复
void APlayerCharacter::AttackRestore()
{
	AttackComponent->ResetAttack();
	StateMachine->SetState(CharacterState::Idle);
	Flipbook->SetLooping(true);
}

//按下跳跃键
void APlayerCharacter::JumpPressed()
{
	Jump();
}


