using UnrealBuildTool;

public class WidgetStudioDemoTarget : TargetRules
{
	public WidgetStudioDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("WidgetStudioDemo");
	}
}
