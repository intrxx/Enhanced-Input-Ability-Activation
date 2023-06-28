// Copyright 2023 Michał Ogiński.


#include "ISAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "ISGameplayTags.h"

UISAssetManager::UISAssetManager()
{
}

UISAssetManager& UISAssetManager::Get()
{
	check(GEngine)
	if(UISAssetManager* Singleton = Cast<UISAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEndine.ini. It must be set to ISAssetManager!"))

	return *NewObject<UISAssetManager>();
}

void UISAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FISGameplayTags::InitializeNativeTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
