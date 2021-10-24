// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_Grapple.h"

#include "../MainCharacter/PK_MainCharacter.h"
#include "../MainCharacter/PK_AutoMoveSystem.h"
#include "../MainCharacter/PK_GrappleSystem.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APK_Grapple::APK_Grapple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger"));
	trigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APK_Grapple::BeginPlay()
{
	Super::BeginPlay();

	onEndMoveWithGrapple.AddLambda([this]()
	{
		EndGrapple();
	});
}

// Called every frame
void APK_Grapple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APK_Grapple::Use(APK_MainCharacter* _user)
{
	Super::Use(_user);
	if (!playerUsingGPE) return;
	InitAutoMove(CreatePath());
	playerUsingGPE->LaunchCharacter(FVector(0, 0, 1), true, true);
}

void APK_Grapple::EndGrapple()
{
	if (!playerUsingGPE) return;
	if(hasGrappleEndForce) playerUsingGPE->LaunchCharacter(CalculEndForce(), true, false);
	ResetPlayer();
}

void APK_Grapple::InitAutoMove(const TArray<FPK_AutoMovePoint> _path)
{
	if (!autoMoveSystemPlayer) return;
	autoMoveSystemPlayer->InitPhysics(0, 2000, true);
	autoMoveSystemPlayer->SetPath(_path);
	autoMoveSystemPlayer->ActivateAutoMove(true);
}

TArray<FPK_AutoMovePoint> APK_Grapple::CreatePath()
{
	if (!playerUsingGPE || !playerCapsule || !hangPoint) return TArray<FPK_AutoMovePoint>();
	TArray<FPK_AutoMovePoint> _path = TArray<FPK_AutoMovePoint>();
	const FVector _hangLocation = hangPoint->GetComponentLocation();
	InitPathPoint(_path, _hangLocation);
	grappleEndVector = _hangLocation - playerUsingGPE->GetActorLocation();
	return _path;
}

void APK_Grapple::InitPathPoint(TArray<FPK_AutoMovePoint>& _path, const FVector _point)
{
	if (!playerCapsule)return;
	FPK_AutoMovePoint _posToGo = FPK_AutoMovePoint();
	_posToGo.Init(FVector(_point.X, _point.Y, _point.Z - playerCapsule->GetScaledCapsuleHalfHeight()), grappleAttractionSpeed, grappleMinDist);
	_posToGo.onPointReached.AddLambda([this]()
	{
		onEndMoveWithGrapple.Broadcast();
	});
	_path.Add(_posToGo);
}

FVector APK_Grapple::CalculEndForce()
{
	if (!playerUsingGPE)return FVector(0);
	if(!grappleSystemPlayer)return playerUsingGPE->GetActorLocation();
	const float _maxSpeed = grappleSystemPlayer->GetMaxSpeedGPE();
	const float _speed = grappleEndVector.Size() > _maxSpeed ? _maxSpeed : grappleEndVector.Size();
	grappleEndVector.Normalize();
	const FVector _forceUp = FVector(0,0, grappleEndVector.Z)* _speed * grappleEndForceUp;
	FVector _forceForward = FVector::VectorPlaneProject(grappleEndVector,FVector::UpVector);
	_forceForward.Normalize();
	return _forceForward* _speed * grappleEndForceForward + _forceUp;
}

