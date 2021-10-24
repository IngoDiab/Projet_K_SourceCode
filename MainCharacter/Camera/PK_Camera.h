// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PK_CameraSettings.h"
#include "PK_Camera.generated.h"

UCLASS()
class PROJET_K_API APK_Camera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APK_Camera();

	UPROPERTY(EditAnywhere)
		class UCameraComponent* cameraComp = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
		FPK_CameraSettings settings;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE UCameraComponent* GetCameraComp() const { return cameraComp; };
	FORCEINLINE void SetTargetToFollow(USceneComponent* _target) { settings.targetToFollow = _target; };
	FORCEINLINE void SetTargetToFocus(USceneComponent* _target) { settings.targetToLookAt = _target; };

public:
	FORCEINLINE void InitCameraSettings(const FPK_CameraSettings& _cameraSettings) { settings = _cameraSettings; };

	void FollowTarget();
	void LookAtTarget();

	FVector GetFinalPosCamera();
	FVector GetFinalRotCamera();

	bool CameraAtPos();
};
