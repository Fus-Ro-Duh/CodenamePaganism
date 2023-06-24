// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CodenamePaganism : ModuleRules
{
	public CodenamePaganism(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
            "EnhancedInput"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(
            new string[]
            {
                "CodenamePaganism/Public/Character",
                "CodenamePaganism/Public/Character/Components",
                "CodenamePaganism/Public/Weapons/Melee"
                //"CodenamePaganism/Public/Weapons/Range"
            });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
