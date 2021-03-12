// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class Act_2D : ModuleRules
{
	public Act_2D(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		//公有依赖模块
		PublicDependencyModuleNames.AddRange
		(
			new string[] 
			{ 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore",
				"Paper2D" 
			}
		);

		//私有依赖模块
		PrivateDependencyModuleNames.AddRange
		(
			new string[] 
			{  
				"SQLite"
			}
		);

		//添加自定义头文件
		PublicIncludePaths.Add( "Act_2D/Public" );
		PublicIncludePaths.Add("ThirdParty/SQLite/Includes");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
