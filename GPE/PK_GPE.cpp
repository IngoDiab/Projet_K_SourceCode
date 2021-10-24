// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_GPE.h"
#include "../MainCharacter/PK_MainCharacter.h"

#include "Components/ShapeComponent.h"
#include "Components/CapsuleComponent.h"

APK_GPE::APK_GPE() 
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void APK_GPE::BeginPlay()
{
	Super::BeginPlay();
	trigger->OnComponentBeginOverlap.AddDynamic(this, &APK_GPE::EnterTrigger);
	trigger->OnComponentEndOverlap.AddDynamic(this, &APK_GPE::ExitTrigger);
}

void APK_GPE::EnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void APK_GPE::ExitTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

APK_MainCharacter* APK_GPE::CheckPlayer(AActor& _otherActor, UPrimitiveComponent& _otherComp) const
{
	APK_MainCharacter* _player = Cast<APK_MainCharacter>(&_otherActor);
	if (!_player)return nullptr;
	const UCapsuleComponent* _capsulePlayer = _player->GetCapsuleComponent();
	const UCapsuleComponent* _overlappedComponent = Cast<UCapsuleComponent>(&_otherComp);
	if (!_overlappedComponent || _overlappedComponent != _capsulePlayer) return nullptr;
	return _player;
}
