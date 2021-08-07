// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DetectiveGameTarget : TargetRules
{
	public DetectiveGameTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		WindowsPlatform.PCHMemoryAllocationFactor = 250;

		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "DetectiveGame" } );
	}
}
