// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"

//构造函数
APlayerCharacterController::APlayerCharacterController()
{
	//绑定代理到动画播放结束
	OnAttackFinishedDelegate.BindDynamic(this, &APlayerCharacterController::AttackRestore);
}

//初始化控制器
void APlayerCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//获取玩家指针和攻击组件
	PlayerCharacter = Cast<APlayerCharacter>(InPawn);
	AttackComponent = PlayerCharacter->GetAttackComponent();

	//添加攻击结束事件
	PlayerCharacter->GetSprite()->OnFinishedPlaying.Add(OnAttackFinishedDelegate);
}

//初始化输入
void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//绑定轴输入
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacterController::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &APlayerCharacterController::MoveUp);

	//攻击键
	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacterController::AttackPresssed);
	InputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacterController::AttackReleased);

	//特殊键
	InputComponent->BindAction("Special", IE_Pressed, this, &APlayerCharacterController::SpecialPresssed);
	InputComponent->BindAction("Special", IE_Released, this, &APlayerCharacterController::SpecialReleased);

	//扳机键
	InputComponent->BindAction("Trigger", IE_Pressed, this, &APlayerCharacterController::TriggerPresssed);
	InputComponent->BindAction("Trigger", IE_Released, this, &APlayerCharacterController::TriggerReleased);

	//跳跃键
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacterController::JumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacterController::JumpReleased);

}

//角色移动
void APlayerCharacterController::MoveRight(float AxisValue)
{
	//当输入操作时
	if (AxisValue != 0)
	{
		//添加左右按键按下状态
		bRightPressed = (fabs(AxisValue - 1.0f) <= eps);
		bLeftPressed = (fabs(AxisValue + 1.0f) <= eps);

		//当非攻击时
		if (AttackComponent->IsMovable())
		{
			//添加移动
			AttackRestore();
			PlayerCharacter->AddMovementInput(FVector(1, 0, 0), AxisValue > 0 ? 1.0f : -1.0f);
			//根据角色方向调整动画
			PlayerCharacter->bFacingRight = AxisValue > 0;
		}

		RecordKeyCombination();
	}
}

void APlayerCharacterController::MoveUp(float AxisValue)
{
	//当输入操作时
	if (AxisValue != 0)
	{
		bUpPressed = (fabs(AxisValue - 1.0f) <= eps);
		bDownPressed = (fabs(AxisValue + 1.0f) <= eps);
		RecordKeyCombination();
	}
}

//按下攻击键
void APlayerCharacterController::AttackPresssed()
{
	bAttackPressed = true;
	RecordKeyCombination();
}

//松开攻击键
void APlayerCharacterController::AttackReleased()
{
	bAttackPressed = false;
	RecordKeyCombination();
}

//按下特殊键
void APlayerCharacterController::SpecialPresssed()
{
	bSpecialPressed = true;
	RecordKeyCombination();
}

//松开特殊键
void APlayerCharacterController::SpecialReleased()
{
	bSpecialPressed = false;
	RecordKeyCombination();
}

//按下扳机键
void APlayerCharacterController::TriggerPresssed()
{
	bTriggerPressed = true;
	RecordKeyCombination();
}

//松开扳机键
void APlayerCharacterController::TriggerReleased()
{
	bTriggerPressed = false;
	RecordKeyCombination();
}

//按下跳跃键
void APlayerCharacterController::JumpPressed()
{
	bJumpPressed = true;
	RecordKeyCombination();

	PlayerCharacter->Jump();
}

//松开跳跃键
void APlayerCharacterController::JumpReleased()
{
	bJumpPressed = false;
	RecordKeyCombination();
}

//记录输入组合
void APlayerCharacterController::RecordKeyCombination()
{
	NextkKeyCombation = FKeyCombination(bAttackPressed, bSpecialPressed, bTriggerPressed, bJumpPressed, bUpPressed, bDownPressed, bLeftPressed, bRightPressed);
	
	//如果包含攻击键
	if (NextkKeyCombation.GetCommand() > 0)
	{
		AddAttackInput();
	}
}

//添加攻击输入
void APlayerCharacterController::AddAttackInput()
{
	//设置延迟接受输入
	auto DelayAttackInput = [&]() -> void
	{
		AttackComponent->SetKeyCombination(NextkKeyCombation);
	};
	auto dlg = FTimerDelegate::CreateLambda(DelayAttackInput);
	GetWorldTimerManager().SetTimer(AttackDelayHandle, dlg, (const float)AttackInputDuration, false);
}


//从攻击恢复
void APlayerCharacterController::AttackRestore()
{
	AttackComponent->ResetAttack();
	PlayerCharacter->UpdateState();
	PlayerCharacter->GetSprite()->SetLooping(true);
	PlayerCharacter->GetSprite()->Play();
}

