// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ISCharacterBase.generated.h"

UCLASS()
class GASINPUTSYSTEM_API AISCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AISCharacterBase();

	// Implementing IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	TWeakObjectPtr<class UISAbilitySystemComponent> AbilitySystemComponent;

};
