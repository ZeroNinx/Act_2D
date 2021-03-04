#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackComponent.generated.h"

//��������
USTRUCT(BlueprintType)
struct FAttackAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UPaperFlipbook* Flipbook;//��������

	UPROPERTY(BlueprintReadWrite)
	int ReadyFrames;//ǰҡ֡��

	UPROPERTY(BlueprintReadWrite)
	int AttackFrames;//�����ж�֡��

	UPROPERTY(BlueprintReadWrite)
	int EndFrames;//��ҡ֡��	

	FAttackAction()
	{
		ReadyFrames = 0;
		AttackFrames = 0;
		EndFrames = 0;
	}

	FAttackAction(int Ready, int Attack, int End)
	{
		ReadyFrames = Ready;
		AttackFrames = Attack;
		EndFrames = End;
	}
};

//�������
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_2D_API UPlayerAttackComponent : public UPaperFlipbookComponent
{
	GENERATED_BODY()

public:	
	
	//���캯��
	UPlayerAttackComponent();

protected:

	//��Ϸ��ʼִ��
	virtual void BeginPlay() override;

public:	

	//Tick����
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
