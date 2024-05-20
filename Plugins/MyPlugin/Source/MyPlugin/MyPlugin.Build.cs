// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyPlugin : ModuleRules
{
	public MyPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"InputCore",
				"EnhancedInput",
				"UMG",
                "Slate",
                "SlateCore",
                 "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "EnhancedInput",
                "HeadMountedDisplay",
                "NavigationSystem",
                "AIModule",
                "Niagara",
                "GameplayTasks",
                "UMG"


            }
            );
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "EnhancedInput",
                "HeadMountedDisplay",
                "NavigationSystem",
                "AIModule",
                "Niagara",
                "GameplayTasks",
                "UMG"

            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				
			}
			);
	}
}
