#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BuildConfigTypes.generated.h"


USTRUCT(BlueprintType)
struct FBuildingApartmentRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
    FString Id;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
    int32 Floor = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
    FString Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
    float Area = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
    FTransform FocusPoint = FTransform::Identity;
};


USTRUCT()
struct FFocusPointJson
{
    GENERATED_BODY()
    UPROPERTY() FVector location = FVector::ZeroVector;
    UPROPERTY() FQuat   rotation = FQuat::Identity;
    UPROPERTY() FVector scale = FVector::OneVector;
};

USTRUCT()
struct FApartmentJson
{
    GENERATED_BODY()
    UPROPERTY() FString id;
    UPROPERTY() FString status;
    UPROPERTY() float area = 0.f;
    UPROPERTY() FFocusPointJson focusPoint;
};

USTRUCT()
struct FFloorJson
{
    GENERATED_BODY()
    UPROPERTY() int32 floor = 0;
    UPROPERTY() TArray<FApartmentJson> apartments;
};

USTRUCT()
struct FBuildingJson
{
    GENERATED_BODY()
    UPROPERTY() TArray<FFloorJson> floors;
};
