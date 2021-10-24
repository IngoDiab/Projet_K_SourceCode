// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PK_CameraSettings.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJET_K_API FPK_CameraSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Target | Follow")
		USceneComponent* targetToFollow = nullptr;

	UPROPERTY(EditAnywhere, Category = "Target | Focus")
		USceneComponent* targetToLookAt = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movement|Position")
		FVector offsetPos = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Movement|Speed")
		float speedMove = 100000;
	UPROPERTY(EditAnywhere, Category = "Movement")
		bool canMove = true;

	UPROPERTY(EditAnywhere, Category = "Focus|Position")
		FVector offsetLookAt = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Focus|Speed")
		float speedLookAt = 100000;
	UPROPERTY(EditAnywhere, Category = "LookAt")
		bool canLookAt = true;
	UPROPERTY(EditAnywhere, Category = "LookAt", meta = (DisplayName = "Tick if look at target (untick = get same rotation)"))
		bool followLook = false;
};
