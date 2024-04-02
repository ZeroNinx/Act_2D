#pragma once

#include "Utils/Types.h"
#include "ActorInterface.generated.h"

UINTERFACE(MinimalAPI,Blueprintable)
class UActorInterface :public UInterface
{
	GENERATED_BODY()
};

class IActorInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void Hit(AActor* Attacker, FAttackProperty AttackProperty);

};
