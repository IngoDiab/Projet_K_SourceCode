// Fill out your copyright notice in the Description page of Project Settings.


#include "PK_Zipline.h"

#include "../MainCharacter/PK_MainCharacter.h"
#include "../MainCharacter/PK_AutoMoveSystem.h"
#include "../MainCharacter/PK_GPEUsingSystem.h"
#include "../MainCharacter/PK_WallRunSystem.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"

// Sets default values
APK_Zipline::APK_Zipline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	begin = CreateDefaultSubobject<USceneComponent>(TEXT("Begin"));
	begin->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	end = CreateDefaultSubobject<USceneComponent>(TEXT("End"));
	end->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	firstPost = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstPost"));
	firstPost->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	secondPost = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondPost"));
	secondPost->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	cable->AttachToComponent(firstPost, FAttachmentTransformRules::KeepRelativeTransform);
	//cable->SetAttachEndToComponent(secondPost);
	
	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	trigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APK_Zipline::BeginPlay()
{
	Super::BeginPlay();
	onZiplineEnd.AddLambda([this]()
	{
		EndZipline();
	});
}

// Called every frame
void APK_Zipline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APK_Zipline::EnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::EnterTrigger(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (!isUsable) return;
	APK_MainCharacter* _player = CheckPlayer(*OtherActor, *OtherComp);
	if (!_player) return;
	UPK_GPEUsingSystem* _gpeSystem = _player->GetGPESystem();
	if (!_gpeSystem) return;
	_gpeSystem->SetCurrentGPE(this);
}

void APK_Zipline::ExitTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::ExitTrigger(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (!isUsable) return;
	APK_MainCharacter* _player = CheckPlayer(*OtherActor, *OtherComp);
	if (!_player) return;
	UPK_GPEUsingSystem* _gpeSystem = _player->GetGPESystem();
	if (!_gpeSystem) return;
	_gpeSystem->SetCurrentGPE(nullptr);
}

void APK_Zipline::CancelCharacterActions()
{
	CancelWallrun();
}

void APK_Zipline::CancelWallrun()
{
	if (!playerUsingGPE) return;
	UPK_WallRunSystem* _wallrun = playerUsingGPE->GetWallRunSystem();
	if (!_wallrun) return;
	_wallrun->QuitWallRun();
	_wallrun->SetCanWallRun(false);
	onZiplineEnd.AddLambda([_wallrun]()
	{
		_wallrun->SetCanWallRun(true);
	});
}

void APK_Zipline::BeginZipline()
{
	if (!playerUsingGPE) return;

	UCapsuleComponent* _capsuleChara = playerUsingGPE->GetCapsuleComponent();
	if (!_capsuleChara) return;

	//InitPhysics(0, 2000, true);
	InitAutoMove(CreatePath(_capsuleChara->GetScaledCapsuleHalfHeight()));
	playerUsingGPE->LaunchCharacter(FVector(0, 0, 1), false, false);
}

void APK_Zipline::EndZipline()
{
	if (!playerUsingGPE) return;
	//InitPhysics(1, 0);
	playerUsingGPE->LaunchCharacter(CalculEndForce(), false, false);
	playerUsingGPE = nullptr;
}

/*void APK_Zipline::InitPhysics(const float _gravityScale, const float _braking, const bool _stopVelocity) const
{
	if (!playerUsingGPE) return;
	UCharacterMovementComponent* _movementCharacter = playerUsingGPE->GetCharacterMovement();
	if (!_movementCharacter) return;
	_movementCharacter->GravityScale = _gravityScale;
	_movementCharacter->BrakingDecelerationFalling = _braking;
	if (_stopVelocity) _movementCharacter->Velocity = FVector(0);
}*/

void APK_Zipline::InitAutoMove(const TArray<FPK_AutoMovePoint> _path)
{
	if (!playerUsingGPE) return;
	UPK_AutoMoveSystem* _autoMoveSystem = playerUsingGPE->GetAutoMoveSystem();
	if (!_autoMoveSystem) return;
	_autoMoveSystem->InitPhysics(0, 2000, true);
	_autoMoveSystem->SetPath(_path);
	_autoMoveSystem->ActivateAutoMove(true);
}

TArray<FPK_AutoMovePoint> APK_Zipline::CreatePath(const float _offset) const
{
	if (!begin || !end) return TArray<FPK_AutoMovePoint>();
	TArray<FPK_AutoMovePoint> _path = TArray<FPK_AutoMovePoint>();

	FPK_AutoMovePoint _firstPoint = FPK_AutoMovePoint();
	_firstPoint.Init(begin->GetComponentLocation() - FVector::UpVector * _offset, ziplineSpeed, ziplineMinDist);
	_path.Add(_firstPoint);

	FPK_AutoMovePoint _secondPoint = FPK_AutoMovePoint();
	_secondPoint.Init(end->GetComponentLocation() - FVector::UpVector * _offset, ziplineSpeed, ziplineMinDist);
	_secondPoint.onPointReached.AddLambda([this]()
	{
		onZiplineEnd.Broadcast();
	});
	_path.Add(_secondPoint);

	return _path;
}

FVector APK_Zipline::CalculEndForce() const
{
	FVector _force = end->GetComponentLocation() - begin->GetComponentLocation();
	_force.Normalize();
	_force *= ziplineSpeed;
	return _force;
}

void APK_Zipline::Use(APK_MainCharacter* _user)
{
	playerUsingGPE = _user;
	if (!playerUsingGPE) return;
	CancelCharacterActions();
	BeginZipline();
}

