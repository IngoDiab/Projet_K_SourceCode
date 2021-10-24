#include "PK_BaseOrbs.h"

#include "Projet_K/MainCharacter/PK_MainCharacter.h"
#include "../../../GPE/PK_GrappleGPE.h"
#include "Projet_K/Utils/Debug.h"

APK_BaseOrbs::APK_BaseOrbs()
{
#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif
	PrimaryActorTick.bCanEverTick = true;
	Awake();
}

void APK_BaseOrbs::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void APK_BaseOrbs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	mDeltaTime = DeltaTime;
// #if WITH_EDITOR
// 	// OnDrawGizmosEvent.Invoke();
// #endif
	OnUpdate.Invoke();
}

void APK_BaseOrbs::BeginDestroy()
{
	Super::BeginDestroy();
	Destroy();
}

bool APK_BaseOrbs::ShouldTickIfViewportsOnly() const
{
	return true;
}

void APK_BaseOrbs::Init()
{
	OnUpdate.Add(this, &APK_BaseOrbs::Shoot);
}

void APK_BaseOrbs::Awake()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	mSphereTrigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	mMeshCollider = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Collider");
	if (mMeshCollider) mMeshCollider->SetupAttachment(RootComponent);
	mSphereTrigger->SetupAttachment(RootComponent);
	mSphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &APK_BaseOrbs::OnBeginOverlap);
	mSphereTrigger->OnComponentEndOverlap.AddDynamic(this, &APK_BaseOrbs::OnEndOverlap);
	OnDrawGizmosEvent.Add(this, &APK_BaseOrbs::OnDrawGizmos);
}

void APK_BaseOrbs::Destroy()
{
	OnTargetInZone.Clear();
	OnUpdate.Clear();
	OnDrawGizmosEvent.Clear();
	OnTakeDamage.Clear();
}

void APK_BaseOrbs::OnDrawGizmos()
{
}

void APK_BaseOrbs::Shoot()
{
}

void APK_BaseOrbs::OnDamage()
{
	OnTakeDamage.Invoke();
	if (!mGrappleOnDeath) return;
	GetWorld()->SpawnActor<APK_GrappleGPE>(mGrapplePrefab, GetActorTransform());
	OnDeath();
	Destroy();
}

void APK_BaseOrbs::OnActorEnterInTrigger(AActor* _actor)
{
}

void APK_BaseOrbs::OnActorExitInTrigger(AActor* _actor)
{
}

void APK_BaseOrbs::OnBeginOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
                                  UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnBeginOverlap"));
	OnActorEnterInTrigger(_otherActor);
	if (!Cast<APK_MainCharacter>(_otherActor))return;
	OnPlayerEnterInTrigger();
}

void APK_BaseOrbs::OnEndOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEndOverlap"));
	OnActorExitInTrigger(_otherActor);
	if (!Cast<APK_MainCharacter>(_otherActor))return;
	OnPlayerExitInTrigger();
}
