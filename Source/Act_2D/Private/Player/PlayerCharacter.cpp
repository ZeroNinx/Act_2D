#include "PlayerCharacter.h"

//重复包含
#include "Kismet/GameplayStatics.h"
#include "PlayerAttackComponent.h"
#include "GlobalBlueprintFunctionLibrary.h"
#include "UIInterface.h"

//构造函数
APlayerCharacter::APlayerCharacter()
{

 	//启用Tick
	PrimaryActorTick.bCanEverTick = true;

	//设定模型
	GetCapsuleComponent()->SetCapsuleHalfHeight(70.0f);
	GetSprite()->SetRelativeLocation(FVector(0.0f, 0.0f, 8.0f));
	GetSprite()->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

	//设定移动组件
	GetCharacterMovement()->GravityScale = 2.5f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 100.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFriction = 8.0f;
	GetCharacterMovement()->AirControl = 1;
	JumpMaxHoldTime = 0.2f;

	//默认角色朝右
	bFacingRight = true;

	//胶囊体组件
	GetCapsuleComponent()->SetCapsuleRadius(23.0f);
	
	//弹簧臂组件
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(FRotator(0, -90.0f,0));//初始化旋转
	SpringArmComponent->TargetArmLength = 900.0f;//初始化长度

	//相机组件
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	//状态机
	StateMachine = CreateDefaultSubobject<UStateMachine>(TEXT("StateMachine"));

	//攻击组件
	AttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("AttackComponent"));
	AttackComponent->SetupAttachment(GetSprite());
	AttackComponent->Setup(this);

	//初始化动画
	InitAnimation();

	//绑定动画
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &APlayerCharacter::OnFlipookFinishedPlaying);


	UClass* MainUIClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UMG/MainUI.MainUI_C'"));
	if (MainUIClass)
	{
		UUserWidget* MainUI = CreateWidget(GetWorld(), MainUIClass);
		if (MainUI)
		{
			UGlobalBlueprintFunctionLibrary::UpdateMainUI(GetWorld(), MainUI);
		}
	}
}

//开始游戏
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGlobalBlueprintFunctionLibrary::UpdatePlayerCharacter(GetWorld(),this);
	UUserWidget* MainUI = UGlobalBlueprintFunctionLibrary::GetMainUI(GetWorld());
	if (MainUI)
	{
		MainUI->AddToViewport();
	}
}


//Tick函数
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//当非战斗时自动调整动画
	if (AttackComponent->AttackID == 0&& GetState() != EState::Hit)
	{
		UpdateDirection();
		UpdateState();
		UpdateAnimation();
	}
	else if (AttackComponent->AttackID == 4)
	{
		//攻击落地强制恢复
		UpdateState();
		if (GetState() == EState::Idle || GetState() == EState::Running)
		{
			AttackComponent->ResetAttack();
			GetSprite()->SetLooping(true);
			GetSprite()->Play();
		}
	}

}

//调整方向
void APlayerCharacter::UpdateDirection()
{
	if (bFacingRight)
	{
		GetSprite()->SetRelativeRotation(FRotator(0, 0, 0));
	}
	else
	{
		GetSprite()->SetRelativeRotation(FRotator(0, 180.0f, 0));
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
		StateMachine->SetState(EState::Jumping);
	}
	else if(Velocy.Z <0)
	{
		StateMachine->SetState(EState::Falling);
	}
	else
	{
		//否则根据X轴速度判断奔跑/静止
		if (UKismetMathLibrary::Abs(Velocy.X) != 0)
		{
			StateMachine->SetState(EState::Running);	
		}
		else
		{
			StateMachine->SetState(EState::Idle);
		}	
	}
}

//初始化动画
void APlayerCharacter::InitAnimation()
{
	IdleFlipbook	= LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Character/PlayerCharacter.PlayerCharacter'"));
	RunningFlipbook = LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Character/Character_Run.Character_Run'"));
	JumpingFlipbook = LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Character/Character_Jump_Start.Character_Jump_Start'"));
	FallingFlipbook = LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Character/Character_Jump_Fall.Character_Jump_Fall'"));
	HitFlipbook		= LoadObject<UPaperFlipbook>(GetWorld(), TEXT("PaperFlipbook'/Game/Paper2D/Character/Hit.Hit'"));
	UpdateAnimation();
}

//调整动画
void APlayerCharacter::UpdateAnimation()
{
	//动画路径
	UPaperFlipbook* AnimationFlipbook=nullptr;
	
	//根据不同状态调整动画
	switch (StateMachine->GetState())
	{
	case EState::Idle:
		AnimationFlipbook = IdleFlipbook;
		break;
	case EState::Running:
		AnimationFlipbook = RunningFlipbook;
		break;
	case EState::Jumping:
		AnimationFlipbook = JumpingFlipbook;
		break;;
	case EState::Falling:
		AnimationFlipbook = FallingFlipbook;
		break;
	}

	//设定动画
	if (!AnimationFlipbook)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Animation Flipbook Failed"));
		return;
	}

	GetSprite()->SetFlipbook(AnimationFlipbook);
}

//单帧动画完成时
void APlayerCharacter::OnFlipookFinishedPlaying()
{
	UpdateState();
	GetSprite()->SetLooping(true);
	GetSprite()->Play();
}

//设置状态
void APlayerCharacter::SetState(EState NewState)
{
	StateMachine->SetState(NewState);
}

//获得状态
EState APlayerCharacter::GetState()
{
	return StateMachine->GetState();
}

bool APlayerCharacter::IsInState(EState InState)
{
	return StateMachine->IsInState(InState);
}

//获得攻击组件
UPlayerAttackComponent* APlayerCharacter::GetAttackComponent()
{
	return AttackComponent;
}

//受击函数
void APlayerCharacter::Hit_Implementation(AActor* Attacker, FAttackProperty AttackProperty)
{
	if (GetState() != EState::Hit)
	{
		GetCharacterMovement()->StopMovementImmediately();
		AttackComponent->ResetAttack();

		//添加瞬时速度
		bFacingRight = Attacker->GetActorLocation().X > GetActorLocation().X;
		UpdateDirection();
		float DirectMark = bFacingRight ? -1.0f : 1.0f;
		float VelocyX = 900.0f * DirectMark;
		GetCharacterMovement()->Velocity = FVector(VelocyX, 0, 0);

		//设定动画
		SetState(EState::Hit);
		GetSprite()->SetLooping(false);
		GetSprite()->SetFlipbook(HitFlipbook);
		GetSprite()->PlayFromStart();

		HealthPoint -= 1;
		
		UUserWidget* MainUI = UGlobalBlueprintFunctionLibrary::GetMainUI(GetWorld());
		if (MainUI)
		{
			IUIInterface::Execute_UpdatePlayerHP(MainUI, HealthPoint);
		}
		
	}
}

void APlayerCharacter::SetGlobalDelay(float Delation, float DelayDuration)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Delation);

	auto RestoreFunction = [&]()->void
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	};
	auto dlg = FTimerDelegate::CreateLambda(RestoreFunction);
	GetWorldTimerManager().SetTimer(HitDelayTimerHandle, dlg, DelayDuration * Delation, false);
}

