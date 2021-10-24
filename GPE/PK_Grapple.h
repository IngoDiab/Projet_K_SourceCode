// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PK_GrappleGPE.h"
#include "PK_Grapple.generated.h"

UCLASS()
class PROJET_K_API APK_Grapple : public APK_GrappleGPE
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Player", meta = (DisplayName = "Player speed"))
	float grappleAttractionSpeed = 2500;
	
	UPROPERTY(EditAnywhere, Category = "Player", meta = (DisplayName = "Min distance to let the player fall"))
	float grappleMinDist = 2000;

	UPROPERTY(EditAnywhere, Category = "Potential Energy", meta = (DisplayName = "Using Energy potential after grapple"))
	bool hasGrappleEndForce = true;

	UPROPERTY(EditAnywhere, Category = "Potential Energy", meta = (DisplayName = "Potential Energy Up"))
	float grappleEndForceUp = 1450;
	
	UPROPERTY(EditAnywhere, Category = "Potential Energy", meta = (DisplayName = "Potential Energy Forward"))
	float grappleEndForceForward = 1450;

	UPROPERTY()
	FVector grappleEndVector;
	
public:	
	// Sets default values for this actor's properties
	APK_Grapple();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Use(APK_MainCharacter* _user) override;
	TArray<struct FPK_AutoMovePoint> CreatePath();
	void InitPathPoint(TArray<FPK_AutoMovePoint>& _path, const FVector _point);
	void InitAutoMove(const TArray<FPK_AutoMovePoint> _path);
	void EndGrapple();
	FVector CalculEndForce();
};
