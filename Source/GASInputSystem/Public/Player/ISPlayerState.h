// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ISPlayerState.generated.h"

class UISAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GASINPUTSYSTEM_API AISPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AISPlayerState();
	
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	UISAbilitySystemComponent* AbilitySystemComponent;
};
