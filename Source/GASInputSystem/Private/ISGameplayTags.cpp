// Copyright 2023 Michał Ogiński.


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
	// Native Input
	AddTag(Input_Move, "Input.Move", "Move input.");
	AddTag(Input_Look, "Input.Look", "Look input.");

	// Ability Input
	AddTag(Ability_Input_Test, "Ability.Input.Test", "Test ability input.");
}

void FISGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
