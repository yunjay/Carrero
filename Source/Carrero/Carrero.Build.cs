using UnrealBuildTool;
using System.IO;

public class Carrero : ModuleRules
{
    public Carrero(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            // Geometry
            "RealtimeMeshComponent",
            "GeometryCore",
            "GeometryFramework",
            "GeometryScriptingCore",
            "DynamicMesh",
            "MeshConversion",
            "MeshDescription",
            "StaticMeshDescription",
            // UI
            "Slate",
            "SlateCore",
            "UMG"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "RenderCore", // shader compilation, etc
            "RHI",
            "Renderer" // Render graph, SceneView*
		});

        #region Assimp
        string AssimpPath = Path.Combine(ModuleDirectory, "../../ThirdParty/Assimp");
        string AssimpIncludePath = Path.Combine(AssimpPath, "include");
        string AssimpLibPath = Path.Combine(AssimpPath, "lib");
        string AssimpBinPath = Path.Combine(AssimpPath, "bin");

        // Include path
        PublicIncludePaths.Add(AssimpIncludePath);
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Link against the import library - preferred in UE
            PublicAdditionalLibraries.Add(Path.Combine(AssimpLibPath, "assimp-vc143-mt.lib"));
            
            // Runtime dependency for DLL
            string AssimpDLL = Path.Combine(AssimpBinPath, "assimp-vc143-mt.dll");
            RuntimeDependencies.Add(AssimpDLL);
            
            // Delay load seems to be best practice
            PublicDelayLoadDLLs.Add("assimp-vc143-mt.dll");
        }
        #endregion

        #region CGAL
        // CGAL (header-only library)
        string CGALPath = Path.Combine(ModuleDirectory, "../../ThirdParty/CGAL");
        string CGALIncludePath = Path.Combine(CGALPath, "include");
        
        // Handles subdirectories conveniently
        PublicIncludePaths.Add(CGALIncludePath);
        #endregion
    }
}
