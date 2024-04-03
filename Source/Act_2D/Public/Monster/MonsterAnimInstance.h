#include "Utils/StateMachine.h"
#include "Monster/Monster.h"
#include "PaperZD/Public/PaperZDAnimInstance.h"
#include "MonsterAnimInstance.generated.h"


UCLASS(Blueprintable)
class UMonsterAnimInstance : public UPaperZDAnimInstance
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual AMonster* GetOwnerMonster();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual EState GetCurrentState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure)
	bool ShouldExitHitState();

private:

	AMonster* OwingMonster = nullptr;

};
