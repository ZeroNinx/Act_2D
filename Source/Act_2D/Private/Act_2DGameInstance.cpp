// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_2DGameInstance.h"
#include "Utils/GlobalBlueprintFunctionLibrary.h"

UAct_2DGameInstance::UAct_2DGameInstance()
{
	UGlobalBlueprintFunctionLibrary::SetGetGameInstance(this);
}

