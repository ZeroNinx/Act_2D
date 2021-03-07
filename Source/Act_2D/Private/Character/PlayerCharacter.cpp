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

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

