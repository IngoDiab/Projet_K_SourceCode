// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_Camera.h"
#include "Camera/CameraComponent.h"
#include "Projet_K/MainCharacter/PK_MainCharacter.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APK_Camera::APK_Camera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APK_Camera::BeginPlay()
{
	Super::BeginPlay();
	cameraComp->PostProcessSettings.bOverride_MotionBlurAmount = 1;
}

// Called every frame
void APK_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FollowTarget();
	LookAtTarget();
}

void APK_Camera::FollowTarget()
{
	if (!settings.targetToFollow || !settings.canMove || CameraAtPos()) return;
	const FVector _newPos = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), GetFinalPosCamera() , GetWorld()->DeltaTimeSeconds, settings.speedMove);
	SetActorLocation(_newPos);
}

void APK_Camera::LookAtTarget()
{
	if (!settings.targetToLookAt || !settings.canLookAt) return;
	if (!settings.followLook) SetActorRotation(settings.targetToFollow->GetComponentRotation());
	else 
	{
		const FRotator _targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetFinalRotCamera());
		const FRotator _newRot = UKismetMathLibrary::RInterpTo_Constant(GetActorRotation(), _targetRot, GetWorld()->DeltaTimeSeconds, settings.speedLookAt);
		SetActorRotation(_newRot);
	}
}

FVector APK_Camera::GetFinalPosCamera()
{
	USceneComponent* _target = settings.targetToFollow;
	if (!_target) return GetActorLocation();
	return _target->GetComponentLocation() + _target->GetForwardVector() * settings.offsetPos.X + _target->GetRightVector() * settings.offsetPos.Y + _target->GetUpVector() * settings.offsetPos.Z;
}

bool APK_Camera::CameraAtPos()
{
	if (!settings.targetToFollow) return true;
	return FVector::DistSquared(GetActorLocation(), GetFinalPosCamera()) < 1;
}

FVector APK_Camera::GetFinalRotCamera()
{
	USceneComponent* _target = settings.targetToLookAt;
	if (!_target) return FVector::ZeroVector;
	return _target->GetComponentLocation() + _target->GetForwardVector() * settings.offsetLookAt.X + _target->GetRightVector() * settings.offsetLookAt.Y + _target->GetUpVector() * settings.offsetLookAt.Z;
}

