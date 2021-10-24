// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PK_GPEUsingSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_K_API UPK_GPEUsingSystem : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
		class APK_MainCharacter* owner = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Interact | GPE", meta = (DisplayName = "Current GPE aiming"))
		class APK_Zipline* currentGPE = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interact", meta = (DisplayName = "Player can interact"))
		bool canInteract = true;

public:	
	// Sets default values for this component's properties
	UPK_GPEUsingSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE void SetCurrentGPE(APK_Zipline* _currentGPE) { currentGPE = _currentGPE; };

public:
	void Interact() const;
};
