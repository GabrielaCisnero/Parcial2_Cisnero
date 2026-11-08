// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Parcial2_Cisnero : ModuleRules
{
	public Parcial2_Cisnero(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Parcial2_Cisnero",
			"Parcial2_Cisnero/Variant_Platforming",
			"Parcial2_Cisnero/Variant_Platforming/Animation",
			"Parcial2_Cisnero/Variant_Combat",
			"Parcial2_Cisnero/Variant_Combat/AI",
			"Parcial2_Cisnero/Variant_Combat/Animation",
			"Parcial2_Cisnero/Variant_Combat/Gameplay",
			"Parcial2_Cisnero/Variant_Combat/Interfaces",
			"Parcial2_Cisnero/Variant_Combat/UI",
			"Parcial2_Cisnero/Variant_SideScrolling",
			"Parcial2_Cisnero/Variant_SideScrolling/AI",
			"Parcial2_Cisnero/Variant_SideScrolling/Gameplay",
			"Parcial2_Cisnero/Variant_SideScrolling/Interfaces",
			"Parcial2_Cisnero/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
