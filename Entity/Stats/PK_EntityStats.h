#pragma once

#include "CoreMinimal.h"
#include "PK_EntityStats.generated.h"

USTRUCT(BlueprintType)
struct PROJET_K_API FPK_EntityStats
{
	GENERATED_BODY()
private:
	UPROPERTY(Category="", EditAnywhere, meta = (DisplayName = "current life", UIMin = 0, UIMax = 1000))
	int mCurrentLife = 100;
	UPROPERTY(Category="", EditAnywhere, meta = (DisplayName = "max life", UIMin = 0, UIMax = 1000))
	int mMaxLife = 100;
public:
	int CurrentLife()const{return mCurrentLife;}
	int MaxLife()const {return mMaxLife;}
};
