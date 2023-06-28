// Copyright 2023 Michał Ogiński.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Characters/ISCharacterBase.h"
#include "ISHeroCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class GASINPUTSYSTEM_API AISHeroCharacter : public AISCharacterBase
{
	GENERATED_BODY()

public:
	AISHeroCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SN|Character", Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;
};
