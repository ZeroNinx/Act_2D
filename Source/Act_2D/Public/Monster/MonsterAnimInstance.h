#pragma once
#include "PaperZDAnimInstance.h"
#include "Utils/StateMachine.h"
#include "MonsterAnimInstance.generated.h"

class AMonster;

UCLASS(Blueprintable)
class UMonsterAnimInstance : public UPaperZDAnimInstance
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual AMonster* GetOwningMonster();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual EState GetCurrentState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure)
	bool ShouldExitHitState();

private:

	AMonster* OwingMonster = nullptr;

};
