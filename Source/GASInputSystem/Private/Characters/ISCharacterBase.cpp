// Copyright 2026 out of sCope team - intrxx


#include "Characters/ISCharacterBase.h"
#include "GAS/ISAbilitySystemComponent.h"

// Sets default values
AISCharacterBase::AISCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

UAbilitySystemComponent* AISCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

// Called when the game starts or when spawned
void AISCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

