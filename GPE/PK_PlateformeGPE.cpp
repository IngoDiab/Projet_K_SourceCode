// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_PlateformeGPE.h"

#include "../MainCharacter/PK_MainCharacter.h"
#include "../MainCharacter/PK_AutoMoveSystem.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APK_PlateformeGPE::APK_PlateformeGPE()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshPlateforme = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshPlateforme"));
	meshPlateforme->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger"));
	trigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APK_PlateformeGPE::BeginPlay()
{
	Super::BeginPlay();
	onEndMoveWithGrapple.AddLambda([this]()
	{
		EndGrapple();
	});

	onEndPlateformeMove.AddLambda([this]()
	{
		isActivated = false;
	});
}

// Called every frame
void APK_PlateformeGPE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovePlateforme();
}

void APK_PlateformeGPE::Use(APK_MainCharacter* _user)
{
	Super::Use(_user);
	posToGo = GetPointToMove();
	MoveCharacter();
	ForbidPlayer(*_user);
	isActivated = true;
	isUsable = false;
}

void APK_PlateformeGPE::EndGrapple()
{
	ResetPlayer();
}

FVector APK_PlateformeGPE::GetPointToMove() const
{
	if (!playerUsingGPE || !playerCapsule) return GetActorLocation();
	const FVector _underUser = playerUsingGPE->GetActorLocation() - FVector::UpVector* playerCapsule->GetScaledCapsuleHalfHeight();
	return (GetActorLocation() + _underUser) / 2.f;
}

void APK_PlateformeGPE::MoveCharacter()
{
	if (!playerUsingGPE || !autoMoveSystemPlayer) return;
	playerUsingGPE->LaunchCharacter(FVector(0, 0, 1), true, false);
	autoMoveSystemPlayer->InitPhysics(0, 2000, true);
	autoMoveSystemPlayer->SetPath(CreatePath());
	autoMoveSystemPlayer->ActivateAutoMove(true);
}

void APK_PlateformeGPE::MovePlateforme()
{
	if (!isActivated) return;
	FVector _newPos = UKismetMathLibrary::VLerp(GetActorLocation(), posToGo, GetWorld()->DeltaTimeSeconds * speedPlateforme);
	SetActorLocation(_newPos);
	if (IsAtPos()) onEndPlateformeMove.Broadcast();
}

TArray<FPK_AutoMovePoint> APK_PlateformeGPE::CreatePath()
{
	if (!playerUsingGPE || !hangPoint || !playerCapsule) return TArray<FPK_AutoMovePoint>();
	TArray<FPK_AutoMovePoint> _path = TArray<FPK_AutoMovePoint>();
	const FVector _posToGo = FVector(posToGo.X, posToGo.Y, posToGo.Z + playerCapsule->GetScaledCapsuleHalfHeight());
	InitPathPoint(_path);
	return _path;
}

void APK_PlateformeGPE::InitPathPoint(TArray<FPK_AutoMovePoint>& _path)
{
	if (!playerCapsule)return;
	FPK_AutoMovePoint _posToGo = FPK_AutoMovePoint();
	_posToGo.Init(FVector(posToGo.X, posToGo.Y, posToGo.Z + playerCapsule->GetScaledCapsuleHalfHeight()), grappleCharacterSpeed, 1);
	_posToGo.onPointReached.AddLambda([this]()
	{
		onEndMoveWithGrapple.Broadcast();
	});
	_path.Add(_posToGo);
}

bool APK_PlateformeGPE::IsAtPos() const
{
	return FVector::Distance(GetActorLocation(), posToGo) < maxDistance;
}


