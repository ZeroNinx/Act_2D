#include "Character/PlayerAttackComponent.h"

//构造函数
UPlayerAttackComponent::UPlayerAttackComponent()
{

	SetActive(true);

}

//攻击开始
void UPlayerAttackComponent::BeginAttack()
{
	UPaperSprite* AttackSprite = LoadObject<UPaperSprite>(GetWorld(), TEXT("PaperSprite'/Game/Paper2D/Character/Attack/Attack_I/Attack_I_Range_Sprite.Attack_I_Range_Sprite'"));
	
	//设置攻击组件
	if (AttackSprite)
	{
		SetSprite(AttackSprite);
	}
	else UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Sprite Failed"));

	//攻击判定
	TSet<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		FString ActorName;
		Actor->GetName(ActorName);
		UKismetSystemLibrary::PrintString(GetWorld(), ActorName);
	}

	//读取数据库


}

