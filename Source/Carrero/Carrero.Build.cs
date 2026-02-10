using UnrealBuildTool;
using System.IO;

public class Carrero : ModuleRules
{
    public Carrero(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Latest;

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
            RuntimeDependencies.Add("$(BinaryOutputDir)/assimp-vc143-mt.dll", AssimpDLL);

            // Delay load seems to be best practice
            PublicDelayLoadDLLs.Add("assimp-vc143-mt.dll");
        }
        #endregion

        #region CGAL
        // CGAL (header-only library)
        string CGALPath = Path.Combine(ModuleDirectory, "../../ThirdParty/CGAL");
        string CGALIncludePath = Path.Combine(CGALPath, "include");
        string CGALGmpIncludePath = Path.Combine(CGALPath, "gmp", "include");

        // Handles subdirectories conveniently
        PublicIncludePaths.Add(CGALIncludePath);
        if (Directory.Exists(CGALGmpIncludePath))
        {
            PublicIncludePaths.Add(CGALGmpIncludePath);
        }
        PublicDefinitions.AddRange(new string[]
            {
            "CGAL_HEADER_ONLY=1",
            "CGAL_NOT_HEADER_ONLY=0"
        });
        #endregion

        #region Boost
        string BoostIncludeDir = System.Environment.GetEnvironmentVariable("BOOST_INCLUDEDIR");
        string BoostLibraryDir = System.Environment.GetEnvironmentVariable("BOOST_LIBRARYDIR");
        if (!string.IsNullOrWhiteSpace(BoostIncludeDir) && Directory.Exists(BoostIncludeDir))
        {
            PublicIncludePaths.Add(BoostIncludeDir);
        }
        if (!string.IsNullOrWhiteSpace(BoostLibraryDir) && Directory.Exists(BoostLibraryDir))
        {
            PublicSystemLibraryPaths.Add(BoostLibraryDir);
        }
        #endregion
    }
}
