// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PK_GPE.h"
#include "PK_Zipline.generated.h"

UCLASS()
class PROJET_K_API APK_Zipline : public APK_GPE
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		float ziplineSpeed = 1000;
	
	UPROPERTY(EditAnywhere)
		float ziplineMinDist = 1000;

	UPROPERTY(EditAnywhere, Category = "Transform")
		USceneComponent* begin = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Transform")
		USceneComponent* end = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* firstPost = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* secondPost = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UCableComponent* cable = nullptr;

	DECLARE_EVENT_OneParam(APK_Zipline, ZiplineBegin, APK_MainCharacter*)
	ZiplineBegin onZiplineBegin;

	DECLARE_EVENT(APK_Zipline, ZiplineEnd)
	ZiplineEnd onZiplineEnd;

public:	
	// Sets default values for this actor's properties
	APK_Zipline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE ZiplineBegin* OnZiplineBegin() { return &onZiplineBegin; };
	FORCEINLINE ZiplineEnd* OnZiplineEnd() { return &onZiplineEnd; };

public:
	virtual void EnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void ExitTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	void CancelCharacterActions();
	void CancelWallrun();

	void BeginZipline();
	void EndZipline();
	
	//void InitPhysics(const float _gravityScale, const float _braking, const bool _stopVelocity = false) const;
	void InitAutoMove(const TArray<struct FPK_AutoMovePoint> _path);
	TArray<FPK_AutoMovePoint> CreatePath(const float _offset) const;
	FVector CalculEndForce() const;

public :
	void Use(APK_MainCharacter* _user) override;
};
