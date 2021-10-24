// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_GPEUsingSystem.h"
#include "PK_MainCharacter.h"
#include "../GPE/PK_Zipline.h"

// Sets default values for this component's properties
UPK_GPEUsingSystem::UPK_GPEUsingSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPK_GPEUsingSystem::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<APK_MainCharacter>(GetOwner());
}


// Called every frame
void UPK_GPEUsingSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPK_GPEUsingSystem::Interact() const
{
	if (!canInteract || !currentGPE) return;
	if (!owner) return;
	currentGPE->Use(owner);
	//IPK_GPE::Execute_Use(currentGPE.GetObject(), owner);
}

