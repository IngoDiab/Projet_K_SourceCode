#include "PK_Bullet.h"

#include "Components/SphereComponent.h"
#include "Projet_K/Interface/Target/PK_Target.h"

APK_Bullet::APK_Bullet()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    //mBulletCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Bullet Collider"));
	//mBulletCollider->SetupAttachment(RootComponent);
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	mMesh->SetupAttachment(RootComponent);
}

void APK_Bullet::BeginPlay()
{
	Super::BeginPlay();
	Init();
	//////
	
	//////
}

void APK_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnUpdate.Invoke();
	OnDrawGizmosEvent.Invoke();
}

void APK_Bullet::BeginDestroy()
{
	UE_LOG(LogTemp, Warning, TEXT("DESTROY"));
	Super::BeginDestroy();
	OnUpdate.Clear();
	OnDrawGizmosEvent.Clear();
}

void APK_Bullet::Init()
{
	InitEvent();
	InitCollider();
}

void APK_Bullet::InitEvent()
{
	OnDrawGizmosEvent.Add(this, &APK_Bullet::OnDrawGizmos);
}

void APK_Bullet::InitCollider()
{
	USphereComponent* _capsule = Cast<USphereComponent>(RootComponent);
	if (!_capsule)return;
	_capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (!mMesh)return;
	mMesh->OnComponentHit.AddDynamic(this, &APK_Bullet::OnBeginHit);
}

void APK_Bullet::OnDrawGizmos()
{
}

void APK_Bullet::OnBeginHit(UPrimitiveComponent* _hitComponent, AActor* _otherActor,
                                UPrimitiveComponent* _otherComp, FVector _normalImpulse, const FHitResult & _hit)
{
	UE_LOG(LogTemp, Warning, TEXT("TOUCH : %s"), *_otherActor->GetName());
	if (!_otherActor) return;
	IPK_Target * _target = Cast<IPK_Target>(_otherActor);
	if (!_target)return;
	_target->OnDamage();
}

