// Copyright 2023 Michał Ogiński.


#include "Characters/Hero/ISHeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ISGameplayTags.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/ISAbilitySystemComponent.h"
#include "GAS/ISAbilitySet.h"
#include "Input/ISInputComponent.h"
#include "Player/ISPlayerController.h"
#include "Player/ISPlayerState.h"

AISHeroCharacter::AISHeroCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 600;
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 70));
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

void AISHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(AISPlayerController* PC = Cast<AISPlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AISHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UISInputComponent* EInputComponent = Cast<UISInputComponent>(PlayerInputComponent);
	check(EInputComponent);
	
	const FISGameplayTags& GameplayTags = FISGameplayTags::Get();
	TArray<uint32> BindHandles;
	
	EInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed,
		&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);

	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Move, ETriggerEvent::Triggered, this,
		&ThisClass::Move);
	EInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Look, ETriggerEvent::Triggered, this,
		&ThisClass::Look);
}

void AISHeroCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>();
	if(GetController())
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardVector, DirectionValue.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, DirectionValue.X);
	}
}

void AISHeroCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	if(GetController())
	{
		if(LookValue.X != 0.0f)
		{
			AddControllerYawInput(LookValue.X);
		}

		if(LookValue.Y != 0.0f)
		{
			AddControllerPitchInput(-LookValue.Y);
		}
	}
}

void AISHeroCharacter::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void AISHeroCharacter::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

void AISHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AISPlayerState* PS = Cast<AISPlayerState>(GetPlayerState());
	check(PS);

	AbilitySystemComponent = Cast<UISAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS,this);

	if(AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
	}
}
