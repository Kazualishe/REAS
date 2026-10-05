// BuildConfigLoader.cpp
#include "BuildConfigLoader.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "AssetToolsModule.h"
#include "Factories/DataTableFactory.h"
#include "Engine/DataTable.h"
#include "EditorAssetLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"

bool UBuildConfigLoader::LoadBuildConfig(const FString& FilePath, TArray<FBuildingApartmentRow>& OutRows)
{
    OutRows.Empty();

    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("[BuildConfig] The file could not be read: %s"), *FilePath);
        return false;
    }

    FBuildingJson Building;
    if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &Building, 0, 0))
    {
        UE_LOG(LogTemp, Error, TEXT("[BuildConfig] Failed to parse JSON"));
        return false;
    }

    for (const FFloorJson& Floor : Building.floors)
    {
        for (const FApartmentJson& Apt : Floor.apartments)
        {
            FBuildingApartmentRow Row;
            Row.Id = Apt.id;
            Row.Floor = Floor.floor;
            Row.Status = Apt.status;
            Row.Area = Apt.area;
            Row.FocusPoint = FTransform(
                Apt.focusPoint.rotation,
                Apt.focusPoint.location,
                Apt.focusPoint.scale
            );
            OutRows.Add(MoveTemp(Row));
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[BuildConfig] Loaded %d apartments"), OutRows.Num());
    return OutRows.Num() > 0;
}

UDataTable* UBuildConfigLoader::CreateAndSaveDataTable(
    const TArray<FBuildingApartmentRow>& Rows,
    const FString& PackagePath,
    const FString& AssetName)
{
    const FString FullObjectPath = FString::Printf(TEXT("%s/%s"), *PackagePath, *AssetName);

    UDataTable* DataTable = nullptr;

    if (UEditorAssetLibrary::DoesAssetExist(FullObjectPath))
    {
        DataTable = Cast<UDataTable>(UEditorAssetLibrary::LoadAsset(FullObjectPath));

        if (!DataTable)
        {
            UE_LOG(LogTemp, Error,
                TEXT("[BuildConfig] Asset '%s' exists, but it is not a DataTable. Overwriting is not possible."),
                *FullObjectPath);
            return nullptr;
        }

        if (DataTable->RowStruct != FBuildingApartmentRow::StaticStruct())
        {
            UE_LOG(LogTemp, Warning,
                TEXT("[BuildConfig] RowStruct for '%s' is different. I’m reassigning it to FBuildingApartmentRow."),
                *FullObjectPath);
            DataTable->RowStruct = FBuildingApartmentRow::StaticStruct();
        }

        DataTable->EmptyTable();

        UE_LOG(LogTemp, Log, TEXT("[BuildConfig] Existing DataTable '%s' has been cleared for rewriting."),
            *FullObjectPath);
    }
    else
    {
        FAssetToolsModule& AssetTools =
            FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

        UDataTableFactory* Factory = NewObject<UDataTableFactory>();
        Factory->Struct = FBuildingApartmentRow::StaticStruct();

        UObject* NewAsset = AssetTools.Get().CreateAsset(
            AssetName,           
            PackagePath,         
            UDataTable::StaticClass(),
            Factory);

        DataTable = Cast<UDataTable>(NewAsset);

        if (!DataTable)
        {
            UE_LOG(LogTemp, Error, TEXT("[BuildConfig] Failed to create DataTable '%s'."),
                *FullObjectPath);
            return nullptr;
        }

        DataTable->RowStruct = FBuildingApartmentRow::StaticStruct();

        UE_LOG(LogTemp, Log, TEXT("[BuildConfig] New created DataTable '%s'."), *FullObjectPath);
    }

    for (const FBuildingApartmentRow& Row : Rows)
    {
        if (Row.Id.IsEmpty()) continue;
        DataTable->AddRow(FName(*Row.Id), Row);
    }

    DataTable->MarkPackageDirty();

    UPackage* Package = DataTable->GetOutermost();
    const FString PackageFileName = FPackageName::LongPackageNameToFilename(
        Package->GetName(), FPackageName::GetAssetPackageExtension());

    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
    SaveArgs.SaveFlags = SAVE_NoError;

    const bool bSaved = UPackage::SavePackage(Package, DataTable, *PackageFileName, SaveArgs);

    if (!bSaved)
    {
        UE_LOG(LogTemp, Warning, TEXT("[BuildConfig] DataTable is created in memory but not saved to disk.: %s"),
            *PackageFileName);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[BuildConfig] DataTable saved: %s"), *PackageFileName);
    }

    return DataTable;
}
