using UnrealBuildTool;
using System.Collections.Generic;

public class AmanecerRojoEditorTarget : TargetRules
{
    public AmanecerRojoEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5Latest;
        ExtraModuleNames.Add("AmanecerRojo");
    }
}
