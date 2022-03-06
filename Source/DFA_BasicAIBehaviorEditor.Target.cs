// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DFA_BasicAIBehaviorEditorTarget : TargetRules
{
	public DFA_BasicAIBehaviorEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "DFA_BasicAIBehavior" } );
	}
}
