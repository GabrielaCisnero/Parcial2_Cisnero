// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PEV2_TF_LeafRace : ModuleRules
{
	public PEV2_TF_LeafRace(ReadOnlyTargetRules Target) : base(Target)
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
		PublicDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore", "Niagara" });

		PublicIncludePaths.AddRange(new string[] {
			"PEV2_TF_LeafRace",
			"PEV2_TF_LeafRace/Variant_Platforming",
			"PEV2_TF_LeafRace/Variant_Platforming/Animation",
			"PEV2_TF_LeafRace/Variant_Combat",
			"PEV2_TF_LeafRace/Variant_Combat/AI",
			"PEV2_TF_LeafRace/Variant_Combat/Animation",
			"PEV2_TF_LeafRace/Variant_Combat/Gameplay",
			"PEV2_TF_LeafRace/Variant_Combat/Interfaces",
			"PEV2_TF_LeafRace/Variant_Combat/UI",
			"PEV2_TF_LeafRace/Variant_SideScrolling",
			"PEV2_TF_LeafRace/Variant_SideScrolling/AI",
			"PEV2_TF_LeafRace/Variant_SideScrolling/Gameplay",
			"PEV2_TF_LeafRace/Variant_SideScrolling/Interfaces",
			"PEV2_TF_LeafRace/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
