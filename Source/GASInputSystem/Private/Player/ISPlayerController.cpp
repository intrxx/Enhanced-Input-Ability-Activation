// Copyright 2023 Michał Ogiński.


#include "Player/ISPlayerController.h"

#include "GAS/ISAbilitySystemComponent.h"
#include "Player/ISPlayerState.h"

AISPlayerController::AISPlayerController()
{
}

AISPlayerState* AISPlayerController::GetISPlayerState() const
{
	return CastChecked<AISPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UISAbilitySystemComponent* AISPlayerController::GetISAbilitySystemComponent() const
{
	const AISPlayerState* PS = GetISPlayerState();
	return CastChecked<UISAbilitySystemComponent>(PS->GetAbilitySystemComponent());
}

void AISPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AISPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if(UISAbilitySystemComponent* ASC = GetISAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
