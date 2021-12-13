// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DetectiveGame : ModuleRules
{
	public DetectiveGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		PublicIncludePaths.AddRange
		(
			new string[]
			{
				"DetectiveGame",
				"DetectiveGame/Public",
				"DetectiveGame/Public/Investigation",
				"DetectiveGame/Public/Investigation/Questions",
				"DetectiveGame/Public/Investigation/Events",
				"DetectiveGame/Public/Investigation/Persons",
				"DetectiveGame/Public/Investigation/Places",		
				"DetectiveGame/Public/Investigation/Evidences",
				"DetectiveGame/Public/Interactable",
				"DetectiveGame/Public/Game",
				"DetectiveGame/Public/UI",
				"DetectiveGame/Public/UI/Investigation"
			}
		);

		PrivateIncludePaths.AddRange
		(
			new string[]
			{
				"DetectiveGame/Private"
			}
		);
	}
}
