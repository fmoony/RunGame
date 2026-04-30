// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RunGame : ModuleRules
{
	public RunGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"FunctionalTesting",
			"GameplayTags"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"RunGame",
			"RunGame/AFunctionalTest",
			"RunGame/Variant_Platforming",
			"RunGame/Variant_Platforming/Animation",
			"RunGame/Variant_Combat",
			"RunGame/Variant_Combat/AI",
			"RunGame/Variant_Combat/Animation",
			"RunGame/Variant_Combat/Gameplay",
			"RunGame/Variant_Combat/Interfaces",
			"RunGame/Variant_Combat/UI",
			"RunGame/Variant_SideScrolling",
			"RunGame/Variant_SideScrolling/AI",
			"RunGame/Variant_SideScrolling/Gameplay",
			"RunGame/Variant_SideScrolling/Interfaces",
			"RunGame/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
