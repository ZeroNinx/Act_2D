using System.IO;
using UnrealBuildTool;

 
public class SQLite : ModuleRules
{
	private string ModulePath
	{
		get
		{
			return ModuleDirectory;
		}
	}

	public SQLite(ReadOnlyTargetRules Target) : base(Target)
	{
        //表示第三方库
	    Type = ModuleType.External;

		//添加包含目录
	    PublicIncludePaths.Add(Path.Combine(ModulePath, "include"));

		//添加静态库
	    PublicAdditionalLibraries.Add(Path.GetFullPath(Path.Combine(ModulePath, "lib","sqlite3.lib")));

	}
}