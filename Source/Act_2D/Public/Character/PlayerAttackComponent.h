#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerAttackComponent.generated.h"

//��������
USTRUCT(BlueprintType)
struct FAttackAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UPaperSprite* AttackSprite;//��������

	UPROPERTY(BlueprintReadWrite)
	int AttackFrames;//�����ж�֡��

};

//�������
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UPlayerAttackComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

public:	
	
	//���캯��
	UPlayerAttackComponent();

	//������ʼ
	UFUNCTION(BlueprintCallable)
	void BeginAttack();

protected:



	
};
