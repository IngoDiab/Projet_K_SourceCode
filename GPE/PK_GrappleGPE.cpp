// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_GrappleGPE.h"
#include "../MainCharacter/PK_MainCharacter.h"
#include "../MainCharacter/PK_AutoMoveSystem.h"
#include "../MainCharacter/PK_GrappleSystem.h"

#include "Components/CapsuleComponent.h"

APK_GrappleGPE::APK_GrappleGPE()
{
	hangPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HangPoint"));
	hangPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void APK_GrappleGPE::BeginPlay()
{
	Super::BeginPlay();

	onAimGrapple.AddLambda([this]() 
	{
		ChangeMaterial();
	});
}

void APK_GrappleGPE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	onAimGrapple.Broadcast();
}

void APK_GrappleGPE::EnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::EnterTrigger(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (!isUsable) return;
	APK_MainCharacter* _player = CheckPlayer(*OtherActor, *OtherComp);
	if (!_player) return;
	AllowPlayer(*_player);
}

void APK_GrappleGPE::ExitTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::ExitTrigger(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	APK_MainCharacter* _player = CheckPlayer(*OtherActor, *OtherComp);
	if (!_player) return;
	ForbidPlayer(*_player);
}

void APK_GrappleGPE::Use(APK_MainCharacter* _user)
{
	playerUsingGPE = _user;
	SavePlayer();
}

void APK_GrappleGPE::SavePlayer()
{
	if (!playerUsingGPE)return;
	playerCapsule = playerUsingGPE->GetCapsuleComponent();
	autoMoveSystemPlayer = playerUsingGPE->GetAutoMoveSystem();
	grappleSystemPlayer = playerUsingGPE->GetGrappleSystem();
}

void APK_GrappleGPE::ResetPlayer()
{
	playerUsingGPE = nullptr;
	playerCapsule = nullptr;
	autoMoveSystemPlayer = nullptr;
	grappleSystemPlayer = nullptr;
}

void APK_GrappleGPE::AllowPlayer(APK_MainCharacter& _player)
{
	UPK_GrappleSystem* _grappleSystem = _player.GetGrappleSystem();
	if (!_grappleSystem) return;
	_grappleSystem->AddInRangeGPE(*this);
}

void APK_GrappleGPE::ForbidPlayer(APK_MainCharacter& _player)
{
	UPK_GrappleSystem* _grappleSystem = _player.GetGrappleSystem();
	if (!_grappleSystem || (_grappleSystem->IsGrappling() && _grappleSystem->GetCurrentGPE() == this)) return;
	_grappleSystem->RemoveInRangeGPE(*this);
	isAimed = false;
}

void APK_GrappleGPE::ChangeMaterial()
{
	if (!materialAimed) return;
	isAimed ? mesh->SetMaterial(0, materialAimed) : mesh->SetMaterial(0, nullptr);
}
