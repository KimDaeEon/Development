// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MMOClient : ModuleRules
{
	public MMOClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		// 아래 쪽에 Sockets 같은 것들 추가해줘야지 FSocket 같은 기능 쓸 수 있다.
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "Networking", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufLib" });

		PrivateIncludePaths.AddRange(new string[] 
		{
            "MMOClient",
            "MMOClient/Network",
			"MMOClient/Game",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
