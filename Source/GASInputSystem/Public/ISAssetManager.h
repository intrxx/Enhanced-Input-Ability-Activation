// Fill out your copyright notice in the Description page of Project Settings.

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
