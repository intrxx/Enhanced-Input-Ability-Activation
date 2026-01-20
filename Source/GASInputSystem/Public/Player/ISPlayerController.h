// Copyright 2026 out of sCope team - intrxx

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ISPlayerController.generated.h"

class UISAbilitySystemComponent;
class AISPlayerState;
/**
 * 
 */
UCLASS()
class GASINPUTSYSTEM_API AISPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AISPlayerController();

	UFUNCTION(BlueprintCallable, Category = "InputSystem|PlayerController")
	AISPlayerState* GetISPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "InputSystem|ISAbilitySystemComponent")
	UISAbilitySystemComponent* GetISAbilitySystemComponent() const;
	
protected:
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
