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
				"Paper2D",
				"SQLite",
				"GameplayTasks",
				"UMG",
                "PaperZD"
            }
		);

		//添加自定义头文件目录
		PublicIncludePaths.AddRange
		(
			new string[]
			{
				"Act_2D/Public",
			}
        );

	}
}
