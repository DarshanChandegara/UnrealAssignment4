// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Assignment4 : ModuleRules
{
	public Assignment4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		//PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" , "EnhancedInput" , "UMG" , "Assignment_1_2" }); 

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Assignment_1_2" , "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "GameplayTasks", "UMG" });

		PublicIncludePaths.AddRange
			(new string[]
			{
					  "C:\\Program Files\\Epic Games\\UE_5.2\\Engine\\Plugins\\EnhancedInput\\Source\\EnhancedInput\\Public" ,
					  "E:\\Unreal Training\\Assignment4\\Plugins\\Assignment_1_2\\Source\\Assignment_1_2\\Public"
				  }) ;

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
