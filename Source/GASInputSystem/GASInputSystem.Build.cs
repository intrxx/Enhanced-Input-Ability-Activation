// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASInputSystem : ModuleRules
{
	public GASInputSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
		
		PrivateDependencyModuleNames.AddRange(new string[] 
		{
			"Slate",
			"SlateCore",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});
	}
}
