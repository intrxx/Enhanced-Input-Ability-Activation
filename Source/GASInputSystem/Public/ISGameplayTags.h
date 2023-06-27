// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;
/**
 * 
 */
struct GASINPUTSYSTEM_API FISGameplayTags
{
public:
    static const FISGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeTags();

public:
	// Native Input
	FGameplayTag Input_Move;
	FGameplayTag Input_Look;

	// Ability Input
	FGameplayTag Ability_Input_Test;

	
protected:
	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
 
private:
    static FISGameplayTags GameplayTags;
};

