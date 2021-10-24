// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PK_GrappleGPE.h"
#include "PK_PlateformeGPE.generated.h"

UCLASS()
class PROJET_K_API APK_PlateformeGPE : public APK_GrappleGPE
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* meshPlateforme = nullptr;

	UPROPERTY(EditAnywhere)
		float grappleCharacterSpeed = 2500;

	UPROPERTY(EditAnywhere)
		float speedPlateforme = 1000;

	UPROPERTY(EditAnywhere)
		float maxDistance = 10;

	UPROPERTY(VisibleAnywhere)
		FVector posToGo = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere)
		bool isActivated = false;

	DECLARE_EVENT(APK_Grapple, EndPlateformeMove)
	EndPlateformeMove onEndPlateformeMove;
	
public:	
	// Sets default values for this actor's properties
	APK_PlateformeGPE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Use(APK_MainCharacter* _user) override;
	void EndGrapple();
	FVector GetPointToMove() const;
	void MoveCharacter();
	void MovePlateforme();
	TArray<struct FPK_AutoMovePoint> CreatePath();
	void InitPathPoint(TArray<FPK_AutoMovePoint>& _path);
	bool IsAtPos() const;
};
