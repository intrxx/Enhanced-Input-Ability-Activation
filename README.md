# Lyra-like Input System 

My implementation of the Lyra Input System for Gameplay Ability System plugin (GAS). In this system, inputs are handled by Gameplay Tags instead of integers. This is not official documentation and neither this project nor myself are affiliated with Epic Games. I make no guarantee for the accuracy of this information.

The goal of this small documentation is to show Lyra's GAS Input System in a small sample project so that the users can learn how everything is done without the need to dig into [Lyra Sample Game](https://docs.unrealengine.com/5.0/en-US/lyra-sample-game-in-unreal-engine/).

The Sample Project and documentation are up-to-date with Unreal Engine 5.2.

The best example of this system and documentation about it will always be [Lyra Sample Game](https://docs.unrealengine.com/5.0/en-US/lyra-sample-game-in-unreal-engine/) and source code.

## Gameplay Ability System

I am going to assume that you have your project with GAS done and working however if you're struggling with getting it to work or don't have a project at all you can switch to [Sample-GAS-Implementation](https://github.com/intrxx/Enhanced-Input-Ability-Activation/tree/Sample-GAS-Implementation) branch and copy the sample project with GAS implemented.

## Gameplay Tags

[Gameplay Tags](https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/Tags/) are hierarchical labels in the form of `Parent.Child.Grandchild...` that are registered with a Tags Manager. Gameplay Tags must be defined ahead of time in the `DefaultGameplayTags.ini`. UE5 Editor provides an interface in project settings to let developers manage GameplayTags without needing to manually edit the `DefaultGameplayTags.ini`. 

![Gameplay Tags Editor in Project Settings](https://github.com/intrxx/Enhanced-Input-Ability-Activation/blob/main/Images/GameplayTagsEditor.png)

More information on Gameplay Tags can be found on [Unreal Engine Documentation](https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/Tags/) or [GASDocumentation](https://github.com/tranek/GASDocumentation) which is a great resource to learn everything related to Gameplay Ability System.

I like to define my Gameplay Tags in c++ to do this you will need to create a class that will define Tags and initializes them with Asset Manager.

```c++
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
	FGameplayTag Input_Move;
	FGameplayTag Input_Look;

	FGameplayTag Ability_Input_WhileInputActive;
	FGameplayTag Ability_Input_OnInputTriggered;

	
protected:
	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
 
private:
	static FISGameplayTags GameplayTags;
};
```


```c++
#include "ISGameplayTags.h"
#include "GameplayTagsManager.h"

FISGameplayTags FISGameplayTags::GameplayTags;

void FISGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

	Manager.DoneAddingNativeTags();
}

void FISGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(Input_Move, "Input.Move", "Move input.");
	AddTag(Input_Look, "Input.Look", "Look input.");

	AddTag(Ability_Input_OnInputTriggered, "Ability.Input.OnInputTriggered", "Test ability input.");
	AddTag(Ability_Input_WhileInputActive, "Ability.Input.WhileInputActive", "Test ability input.");
}

void FISGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
```

You will also need to override the `StartInitialLoading()` function in Asset Manager and call `InitializeNativeTags()`.

```c++
void UISAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FISGameplayTags::InitializeNativeTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
```
Lastly, you will need to add the custom Asset Manager class in Editor.

![Adding Asset Manager class in Editor](https://github.com/intrxx/Enhanced-Input-Ability-Activation/blob/main/Images/AddingAssetManagerInEditor.png)

## Implementing the Lyra-like Input System - Code

### Input Config

This Data Asset will hold mappings between Input Tag and Input Actions.

```c++
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ISInputConfig.generated.h"

class UInputAction;
/**
 * FISInputAction
 *
 *	Struct used to map an input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FISInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * 
 */
UCLASS()
class GASINPUTSYSTEM_API UISInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FISInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FISInputAction> AbilityInputActions;
};
```


```c++
#include "Input/ISInputConfig.h"

const UInputAction* UISInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FISInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}

const UInputAction* UISInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FISInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}
```

### Ability Set / Ability Input Config

Similar Data Asset that holds mappings between Abilities and Gameplay Tags, it's also a great place to hold Gameplay Abilities, Gameplay Effects, or Attribute Sets that will be given to the Player, in this example we only focus on Gameplay Abilities.

```c++
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "ISAbilitySet.generated.h"

class UISAbilitySystemComponent;
class UISGameplayAbility;
/**
 * FISAbilitySet_GameplayAbility
 *
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FISAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UISGameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * FISAbilitySet_GrantedHandles
 *
 *	Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FISAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};


/**
 * 
 */
UCLASS()
class GASINPUTSYSTEM_API UISAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	void GiveToAbilitySystem(UISAbilitySystemComponent* InASC, FISAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FISAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
```

```c++
#include "GAS/ISAbilitySet.h"
#include "GameplayAbilitySpecHandle.h"
#include "GAS/ISAbilitySystemComponent.h"
#include "GAS/ISGameplayAbility.h"


void FISAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void UISAbilitySet::GiveToAbilitySystem(UISAbilitySystemComponent* InASC,
	FISAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(InASC);

	if (!InASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FISAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UISGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UISGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);
		
		const FGameplayAbilitySpecHandle AbilitySpecHandle = InASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}
```

### Enhanced Input Component 

Now we need to create functions to add bindings to Ability and Native Actions with Gameplay Tags in our custom Enhanced Input Component.

```c++
#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "ISInputConfig.h"
#include "ISInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASINPUTSYSTEM_API UISInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	UISInputComponent();

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UISInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UISInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template<class UserClass, typename FuncType>
void UISInputComponent::BindNativeAction(const UISInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UISInputComponent::BindAbilityActions(const UISInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);
	for (const FISInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
```

You will also need to add the custom Enhanced Input Component in the Editor to replace the default one.

![Adding custom Input Compoenent in Editor](https://github.com/intrxx/Enhanced-Input-Ability-Activation/blob/main/Images/AddingDefaultInputComponentClass.png)

### Gameplay Ability

In our custom Gameplay Ability class, we will create enum class so we can determine if the Ability needs to be fired `OnInputTriggered` or `WhileInputActive`.

```c++
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
	OnInputTriggered,
	WhileInputActive
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
```

### Ability System Component

Custom ASC where we create functions to process ability input and hold Spec Handles.

```c++
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ISAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASINPUTSYSTEM_API UISAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UISAbilitySystemComponent();

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

protected:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
```


```c++
#include "GAS/ISAbilitySystemComponent.h"
#include "GAS/ISGameplayAbility.h"

UISAbilitySystemComponent::UISAbilitySystemComponent()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UISAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UISAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UISAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();
	
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UISGameplayAbility* SNAbility = CastChecked<UISGameplayAbility>(AbilitySpec->Ability);

				if (SNAbility->GetActivationPolicy() == EISAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UISGameplayAbility* SNAbility = CastChecked<UISGameplayAbility>(AbilitySpec->Ability);

					if (SNAbility->GetActivationPolicy() == EISAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}
	
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UISAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UISAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);
	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UISAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);
	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}
```

### Player Controller

Our custom Player Controller needs to override `PostProcessInput()` function and call `ProcessAbilityInput()` from our Ability System Component:

```c++
virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
```

```c++
void AISPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if(UISAbilitySystemComponent* ASC = GetISAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
```

### Character Class

In this class, we will bind input and add Abilities from our AbilitySet Data Asset. We also need to create functions to call `InputAbilityInputTagPressed()` and `InputAbilityInputTagReleased()` from our Ability System Component.

```c++
virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

virtual void PossessedBy(AController* NewController) override;

void InputAbilityInputTagPressed(FGameplayTag InputTag);
void InputAbilityInputTagReleased(FGameplayTag InputTag);

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Abilities")
UISAbilitySet* AbilitySet;
	
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Input")
UISInputConfig* InputConfig;

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Input")
class UInputMappingContext* DefaultMappingContext;
```


```c++
void AISHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UISInputComponent* ISInputComponent = Cast<UISInputComponent>(PlayerInputComponent);
	check(ISInputComponent);
	
	const FISGameplayTags& GameplayTags = FISGameplayTags::Get();
	TArray<uint32> BindHandles;
	
	ISInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed,
		&ThisClass::InputAbilityInputTagReleased, /*out*/ BindHandles);

	ISInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Move, ETriggerEvent::Triggered, this,
		&ThisClass::Move);
	ISInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Look, ETriggerEvent::Triggered, this,
		&ThisClass::Look);
}

void AISHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AISPlayerState* PS = Cast<AISPlayerState>(GetPlayerState());
	check(PS);

	AbilitySystemComponent = Cast<UISAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	AbilitySystemComponent->InitAbilityActorInfo(PS,this);

	if(AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), nullptr, this);
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
```

## Implementing the Lyra-like Input System - Editor

### Gameplay Ability

We are going to create Gameplay Ability derived from our `ISGameplayAbility` custom class. This simple Gameplay Ability will contain the `Wait Input Release` task to check if our `WhileInputActive` activation works correctly. The sample project contains one more Gameplay Ability with a simple OnInputTrigger activation policy.

![WaitInputReleaseGameplayAbilityBlueprint](https://github.com/intrxx/Enhanced-Input-Ability-Activation/blob/main/Images/GameplayAbility.png)

### Input Actions

For every Ability/Native input, we need to create an Input Action that will be mapped to Gameplay Tag.

![InputActionDataAsset](https://github.com/intrxx/Enhanced-Input-Ability-Activation/blob/main/Images/InputAction.png)

### Input Mapping Context

Standard Input Mapping Context that will map our Input Actions to control bindings.

![InputMappingContextDataAsset](https://github.com/intrxx/Enhanced-Input-Ability-Activation/blob/main/Images/InputMappingContext.png)

### Input Config and Ability Set Data Assets

Now we need to create two Data Assets derived from the ones we created in the code.

![InputConfigDataAsset](https://github.com/intrxx/Enhanced-Input-Ability-Activation/blob/main/Images/InputConfig.png)
#### InputConfig

![AbilitySetDataAsset](https://github.com/intrxx/Enhanced-Input-Ability-Activation/blob/main/Images/AbilitySet.png)
#### AbilitySet

### Character Class Blueprint

Lastly, we need to add all the Data Assets to our Character Class.

![CharacterClassBlueprint](https://github.com/intrxx/Enhanced-Input-Ability-Activation/blob/main/Images/CharacterClass.png)













