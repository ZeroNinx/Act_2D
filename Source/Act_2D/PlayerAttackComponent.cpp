#include "PlayerAttackComponent.h"

//���캯��
UPlayerAttackComponent::UPlayerAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	//��������
	SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

}


//��Ϸ��ʼʱִ��
void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

//Tick����
void UPlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

