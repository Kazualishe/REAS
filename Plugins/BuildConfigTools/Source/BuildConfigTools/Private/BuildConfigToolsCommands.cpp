// Copyright Epic Games, Inc. All Rights Reserved.

#include "BuildConfigToolsCommands.h"

#define LOCTEXT_NAMESPACE "FBuildConfigToolsModule"

void FBuildConfigToolsCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "BuildConfigTools", "Bring up BuildConfigTools window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
