// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ISGameplayAbility.generated.h"

/**
 * Defines how ability is meant to activate
 */
UENUM(BlueprintType)
enum class EISAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,
	
	// Continually try to activate the ability while the input is active.
	WhileInputActive,
	
	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

/**
 * 
 */
UCLASS()
class GASINPUTSYSTEM_API UISGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	EISAbilityActivationPolicy GetActivationPolicy() const {return ActivationPolicy;}
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SN|Ability Activation")
	EISAbilityActivationPolicy ActivationPolicy;
	
};
