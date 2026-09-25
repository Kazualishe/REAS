// BuildConfigWidget.h
#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BuildConfigTypes.h"
#include "BuildConfigWidget.generated.h"

UCLASS(meta = (IsBlueprintBase = true))
class REAS_API UBuildConfigWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Build Config")
    UDataTable* LoadBuildConfigAndCreateTable(
        const FString& FilePath,
        const FString& PackagePath,
        const FString& AssetName);

    UFUNCTION(BlueprintCallable, Category = "Build Config")
    bool LoadBuildConfigRows(const FString& FilePath, TArray<FBuildingApartmentRow>& OutRows);

    UPROPERTY(BlueprintReadOnly, Category = "Build Config")
    TObjectPtr<UDataTable> LastLoadedTable = nullptr;
};
