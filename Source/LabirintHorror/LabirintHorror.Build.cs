// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LabirintHorror : ModuleRules
{
	public LabirintHorror(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem",  "UMG" });
	}
}
