// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

}

//按键绑定
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//按下攻击
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::AttackPresssed);
}

//按下攻击键
void APlayerCharacter::AttackPresssed()
{

	//改变状态为攻击
	StateMachine->SetState(CharacterState::Attacking);
	Flipbook->SetLooping(false);
	AttackComponent->Attack();

	//设置延迟
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "AttackRestore";
	UKismetSystemLibrary::Delay(this,Flipbook->GetFlipbookLength(),LatentInfo);


}

//攻击恢复
void APlayerCharacter::AttackRestore()
{
	AttackComponent->ResetAttack();
	StateMachine->SetState(CharacterState::Idle);
	Flipbook->SetLooping(true);
}
