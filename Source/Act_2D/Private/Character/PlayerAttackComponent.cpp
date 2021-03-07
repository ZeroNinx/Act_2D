#include "Character/PlayerAttackComponent.h"

//���캯��
UPlayerAttackComponent::UPlayerAttackComponent()
{

	SetActive(true);

}

//������ʼ
void UPlayerAttackComponent::BeginAttack()
{
	UPaperSprite* AttackSprite= LoadObject<UPaperSprite>(GetWorld(), TEXT("PaperSprite'/Game/Paper2D/Character/Attack/Attack_I/Attack_I_Range_Sprite.Attack_I_Range_Sprite'"));
	
	//���ù������
	if (AttackSprite)
	{
		SetSprite(AttackSprite);
	}
	else UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Load Sprite Failed"));

	TSet<AActor*> O;
	GetOverlappingActors(O);
	for (AActor* i :O)
	{
		FString s;
		i->GetName(s);
		UKismetSystemLibrary::PrintString(GetWorld(), s);
	}
}

