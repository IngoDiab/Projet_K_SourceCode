// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PK_GPE.h"
#include "PK_GrappleGPE.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT)
class PROJET_K_API APK_GrappleGPE : public APK_GPE
{
	GENERATED_BODY()

protected :

	UPROPERTY()
		class UPK_AutoMoveSystem* autoMoveSystemPlayer = nullptr;

	UPROPERTY()
		class UPK_GrappleSystem* grappleSystemPlayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mesh = nullptr;

	UPROPERTY(EditAnywhere)
	USceneComponent* hangPoint = nullptr;

	UPROPERTY(EditAnywhere)
	bool isAimed = false;

	UPROPERTY(EditAnywhere)
	UMaterial* materialAimed = nullptr;

	DECLARE_EVENT(APK_Grapple, AimGrapple)
	AimGrapple onAimGrapple;

	DECLARE_EVENT(APK_Grapple, EndMoveWithGrapple)
	EndMoveWithGrapple onEndMoveWithGrapple;

public:
	APK_GrappleGPE();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	FORCEINLINE EndMoveWithGrapple* OnEndMoveWithGrapple() { return &onEndMoveWithGrapple; };
	FORCEINLINE USceneComponent* GetHangPoint() const { return hangPoint; };
	FORCEINLINE void SetAimed(const bool _aimed) { isAimed = _aimed; };
	FORCEINLINE bool IsAimed() const { return isAimed; };

public:
	virtual void EnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void ExitTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	void Use(APK_MainCharacter* _user) override;
	void SavePlayer();
	void ResetPlayer();
	void AllowPlayer(APK_MainCharacter& _player);
	void ForbidPlayer(APK_MainCharacter& _player);
	void ChangeMaterial();
};
