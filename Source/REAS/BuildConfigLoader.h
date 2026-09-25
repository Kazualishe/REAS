// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BuildConfigTypes.h"
#include "BuildConfigLoader.generated.h"

/**
 * 
 */
UCLASS()
class REAS_API UBuildConfigLoader : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "Build Config")
    static bool LoadBuildConfig(const FString& FilePath, TArray<FBuildingApartmentRow>& OutRows);

    UFUNCTION(BlueprintCallable, Category = "Build Config")
    static UDataTable* CreateAndSaveDataTable(
        const TArray<FBuildingApartmentRow>& Rows,
        const FString& PackagePath,
        const FString& AssetName   
    );
};
