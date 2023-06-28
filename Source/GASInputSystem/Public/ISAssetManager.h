// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ISAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GASINPUTSYSTEM_API UISAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UISAssetManager();

	static UISAssetManager& Get();

	virtual void StartInitialLoading() override;
};
