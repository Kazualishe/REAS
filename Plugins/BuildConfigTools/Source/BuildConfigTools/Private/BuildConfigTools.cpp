// Copyright Epic Games, Inc. All Rights Reserved.

#include "BuildConfigTools.h"
#include "BuildConfigToolsStyle.h"
#include "BuildConfigToolsCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName BuildConfigToolsTabName("BuildConfigTools");

#define LOCTEXT_NAMESPACE "FBuildConfigToolsModule"

void FBuildConfigToolsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FBuildConfigToolsStyle::Initialize();
	FBuildConfigToolsStyle::ReloadTextures();

	FBuildConfigToolsCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FBuildConfigToolsCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FBuildConfigToolsModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FBuildConfigToolsModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(BuildConfigToolsTabName, FOnSpawnTab::CreateRaw(this, &FBuildConfigToolsModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FBuildConfigToolsTabTitle", "BuildConfigTools"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FBuildConfigToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FBuildConfigToolsStyle::Shutdown();

	FBuildConfigToolsCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(BuildConfigToolsTabName);
}

TSharedRef<SDockTab> FBuildConfigToolsModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FBuildConfigToolsModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("BuildConfigTools.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FBuildConfigToolsModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(BuildConfigToolsTabName);
}

void FBuildConfigToolsModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FBuildConfigToolsCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FBuildConfigToolsCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBuildConfigToolsModule, BuildConfigTools)