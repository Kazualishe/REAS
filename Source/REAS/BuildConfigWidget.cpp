// BuildConfigWidget.cpp
#include "BuildConfigWidget.h"
#include "BuildConfigLoader.h"

UDataTable* UBuildConfigWidget::LoadBuildConfigAndCreateTable(
    const FString& FilePath, const FString& PackagePath, const FString& AssetName)
{
    TArray<FBuildingApartmentRow> Rows;
    if (!UBuildConfigLoader::LoadBuildConfig(FilePath, Rows))
    {
        return nullptr;
    }

    LastLoadedTable = UBuildConfigLoader::CreateAndSaveDataTable(Rows, PackagePath, AssetName);
    return LastLoadedTable;
}

bool UBuildConfigWidget::LoadBuildConfigRows(const FString& FilePath, TArray<FBuildingApartmentRow>& OutRows)
{
    return UBuildConfigLoader::LoadBuildConfig(FilePath, OutRows);
}
