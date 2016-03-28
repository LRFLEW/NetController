// Some copyright should be here...

using UnrealBuildTool;

public class NetController : ModuleRules
{
	public NetController(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"NetController/Public",
				
				// ... add other public include paths required here ...
			}
			);
		
		PublicIncludePathModuleNames.AddRange(
			new string[] {
				"InputDevice",
			
				// ... add public include path modules required here ...
			}
			);
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"NetController/Private",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				
				// ... add public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"InputCore",
				"Sockets",
				
				// ... add other private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
