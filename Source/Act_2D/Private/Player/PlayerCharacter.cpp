#include "PlayerCharacter.h"

//重复包含
#include "PlayerAttackComponent.h"

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
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 100.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFriction = 8.0f;
	GetCharacterMovement()->AirControl = 1;
	JumpMaxHoldTime = 0.2f;

	//默认角色朝右
	bFacingRight = true;
	
	//弹簧臂组件
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(FRotator(0, -90.0f,0));//初始化旋转
	SpringArmComponent->TargetArmLength = 700.0f;//初始化长度

	//相机组件
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	//状态机
	StateMachine = CreateDefaultSubobject<UStateMachine>(TEXT("StateMachine"));

	//攻击组件
	AttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("AttackComponent"));
	AttackComponent->SetupAttachment(GetSprite());
	AttackComponent->Setup(this);

	//为了而在编辑器中方便显示动画
	UpdateAnimation();
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
	if (AttackComponent->AttackID == 0)
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

//调整动画
void APlayerCharacter::UpdateAnimation()
{
	//动画路径
	FString AnimationFlipbook;
	
	//根据不同状态调整动画
	switch (StateMachine->GetState())
	{
	case EState::Idle:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Character/PlayerCharacter.PlayerCharacter'";
		break;
	case EState::Running:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Character/Run/Character_Run.Character_Run'";
		break;
	case EState::Jumping:
		AnimationFlipbook = "PaperFlipbook'/Game/Paper2D/Character/Jump/Character_Jump_Start.Character_Jump_Start'";
		break;;
	case EState::Falling:
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

	GetSprite()->SetFlipbook(NewAnimation);
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

//获得攻击组件
UPlayerAttackComponent* APlayerCharacter::GetAttackComponent()
{
	return AttackComponent;
}

//受击函数
void APlayerCharacter::Hit(FAttackProperty HitAttackProperty)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString("Hit OK"));

}

