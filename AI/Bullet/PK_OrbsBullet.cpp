#include "PK_OrbsBullet.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projet_K/Utils/Debug.h"

APK_OrbsBullet::APK_OrbsBullet()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	mSphereCollider = CreateDefaultSubobject<USphereComponent>("Sphere Collider");
	mSphereCollider->SetupAttachment(RootComponent);
	mSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &APK_OrbsBullet::OnBeginOverlap);
}

void APK_OrbsBullet::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void APK_OrbsBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	mDeltaTime = DeltaTime;
	OnUpdate.Invoke();
}

void APK_OrbsBullet::BeginDestroy()
{
	Super::BeginDestroy();
	OnUpdate.Clear();
}

void APK_OrbsBullet::Init()
{
	OnUpdate.Add(this, &APK_OrbsBullet::MoveTo);
}

void APK_OrbsBullet::OnBeginOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	UGameplayStatics::ApplyDamage(_otherActor, 50, nullptr, this, nullptr);
	// Destroy();
}

void APK_OrbsBullet::SetEndLocation(const FVector& _location)
{
	mEndLocation = _location;
}

void APK_OrbsBullet::MoveTo()
{
	const FVector _newLocation = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), mEndLocation, mDeltaTime, mSpeed);
	// SetActorLocation(_newLocation);
	mSphereCollider->AddForce(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mEndLocation).Vector() * mSpeed * mDeltaTime * 10);
}

