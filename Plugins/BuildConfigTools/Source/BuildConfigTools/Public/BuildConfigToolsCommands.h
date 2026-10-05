// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "BuildConfigToolsStyle.h"

class FBuildConfigToolsCommands : public TCommands<FBuildConfigToolsCommands>
{
public:

	FBuildConfigToolsCommands()
		: TCommands<FBuildConfigToolsCommands>(TEXT("BuildConfigTools"), NSLOCTEXT("Contexts", "BuildConfigTools", "BuildConfigTools Plugin"), NAME_None, FBuildConfigToolsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};