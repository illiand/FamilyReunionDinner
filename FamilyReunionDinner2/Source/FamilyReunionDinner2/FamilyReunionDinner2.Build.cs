// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FamilyReunionDinner2 : ModuleRules
{
	public FamilyReunionDinner2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Json", "LevelSequence", "MovieScene" });
	}
}
