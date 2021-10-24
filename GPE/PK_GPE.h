// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PK_GPE.generated.h"

UCLASS(ABSTRACT)
class PROJET_K_API APK_GPE : public AActor
{
	GENERATED_BODY()

protected :
	UPROPERTY()
		bool isUsable = true;

	UPROPERTY()
		class APK_MainCharacter* playerUsingGPE = nullptr;

	UPROPERTY()
		class UCapsuleComponent* playerCapsule = nullptr;


	UPROPERTY(EditAnywhere)
		class UShapeComponent* trigger = nullptr;

public:
	APK_GPE();
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void EnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void ExitTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual void Use(APK_MainCharacter* _user) PURE_VIRTUAL(APK_GPE::Use, );
	virtual APK_MainCharacter* CheckPlayer(AActor& _otherActor, UPrimitiveComponent& _otherComp) const;
};
