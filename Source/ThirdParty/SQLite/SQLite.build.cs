using System.IO;
using UnrealBuildTool;

 
public class SQLite : ModuleRules
{
	public SQLite(ReadOnlyTargetRules Target) : base(Target)
	{
        //表示第三方库
	    Type = ModuleType.External;
		 		
		//添加包含目录
	    PublicIncludePaths.Add("Includes/");
 
		//添加库目录
	    PublicAdditionalLibraries.Add("/Libs/sqlite3.lib");
 
	}
}