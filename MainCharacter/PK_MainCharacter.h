// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Projet_K/MainCharacter/Camera/PK_CameraSettings.h"
#include "PK_MainCharacter.generated.h"

UCLASS()
class PROJET_K_API APK_MainCharacter : public ACharacter
{
	GENERATED_BODY()
/* TEST */
private:
	FTimerHandle mHandleSlowMo;
	UPROPERTY(Category="Main Chara | Settings", EditAnywhere, meta = (DisplayName = "Cooldown for reset slow mo"))
		float mCooldownSlowMo = 0.5f;
	bool mIsInCooldownSlowMo = false;
public:
	bool IsInCooldownSlowMo()const;
	void SetCooldownForSlowMo();
	UFUNCTION() void ResetCooldownForSlowMo();
/* END TEST*/
protected:
	UPROPERTY()
		class UPK_MCAnimation* animations = nullptr;

	UPROPERTY()
		class APK_HUD* hud = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Controller", meta = (DisplayName = "Playing with Mouse/Keyboard"))
		bool isKeyboard = true;

	UPROPERTY(VisibleAnywhere, Category = "Falling", meta = (DisplayName = "Player is falling"), BlueprintReadWrite)
		bool isFalling = false;

	UPROPERTY(VisibleAnywhere, Category = "Animations", meta = (DisplayName = "Player is falling"), BlueprintReadWrite)
		bool isShooting = false;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Camera player settings"))
		FPK_CameraSettings cameraSettings;

	UPROPERTY(EditAnywhere, Category = "Jump Character", meta = (DisplayName = "Max number jump allowed"))
		int countJumpMax = 1;

	UPROPERTY(VisibleAnywhere, Category = "Jump Character", meta = (DisplayName = "Player is jumping"), BlueprintReadWrite)
		bool isJumping = false;

	UPROPERTY()
		int countJump = 0;

	UPROPERTY(EditAnywhere, Category = "Speed Character", meta = (DisplayName = "Player max speed"))
		float characterMaxSpeed = 1250;

	UPROPERTY(EditAnywhere, Category = "Speed Character", meta = (DisplayName = "Player max acceleration"))
		float characterMaxAcceleration = 2048;

	UPROPERTY(EditAnywhere, Category = "Constraints", meta = (DisplayName = "Player gravity"))
		float characterGravity = 2;

	UPROPERTY(EditAnywhere, Category = "Caracteristics", meta = (DisplayName = "Player height"))
		float baseHeight = 88;

	UPROPERTY(EditAnywhere, Category = "Constraints", meta = (DisplayName = "Player friction"))
		float baseFriction = 8;

	UPROPERTY(EditAnywhere, Category = "Constraints", meta = (DisplayName = "Player braking"))
		float baseBraking = 2048;
	
	UPROPERTY(EditAnywhere, Category = "Constraints", meta = (DisplayName = "Player braking on fall"))
		float baseBrakingFalling = 0;

	UPROPERTY()
		TArray<AActor*> ignore = TArray<AActor*>();
	
	UPROPERTY()
		UCharacterMovementComponent* moveComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movements", BlueprintReadOnly)
		class UPK_MovementBehaviour* movementBehaviour = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movements", BlueprintReadOnly)
		class UPK_WallRunSystem* wallRunSystem = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movements", BlueprintReadOnly)
		class UPK_SlideSystem* slideSystem = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Movements", BlueprintReadOnly)
		class UPK_ClimbSystem* climbSystem = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movements", BlueprintReadOnly)
		class UPK_AutoMoveSystem* autoMoveSystem = nullptr;

	UPROPERTY(EditAnywhere, Category = "Shoot", BlueprintReadOnly)
		class UPK_ShootSystem* shootSystem = nullptr;

	UPROPERTY(EditAnywhere, Category = "Power", BlueprintReadOnly)
		class UPK_TimeDilationPower* timeDilationPower = nullptr;

	UPROPERTY(EditAnywhere, Category = "GPE", BlueprintReadOnly)
		class UPK_GPEUsingSystem* GPESystem = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Grapple", BlueprintReadOnly)
		class UPK_GrappleSystem* grappleSystem = nullptr;

	UPROPERTY(EditAnywhere, Category = "Component | Grapple", BlueprintReadOnly)
		class UCableComponent* cable = nullptr;

	UPROPERTY(EditAnywhere, Category = "Component | Camera", BlueprintReadOnly)
		class USceneComponent* cameraSlot = nullptr;

	UPROPERTY(EditAnywhere, Category = "Component | Grapple", BlueprintReadOnly)
		class USceneComponent* endCable = nullptr;
	
	//CAMERA MANAGER
	UPROPERTY(EditAnywhere, Category = "Component | Camera")
		class APK_Camera* camera = nullptr;

	DECLARE_EVENT_OneParam(APK_MainCharacter, UpdateCharacter, float)
	UpdateCharacter onUpdate;

public:
	// Sets default values for this character's properties
	APK_MainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE bool GetIsKeyboard() const { return isKeyboard; };
	FORCEINLINE bool GetIsFalling() const { return isFalling; };
	FORCEINLINE bool IsJumping() const { return isJumping; };

	FORCEINLINE UpdateCharacter* OnUpdate() { return &onUpdate; };

	FORCEINLINE UPK_MovementBehaviour* GetMovementBehaviour() const { return movementBehaviour; };
	FORCEINLINE UPK_WallRunSystem* GetWallRunSystem() const { return wallRunSystem; };
	FORCEINLINE UPK_SlideSystem* GetSlideSystem() const { return slideSystem; };
	FORCEINLINE UPK_ClimbSystem* GetClimbSystem() const { return climbSystem; };
	FORCEINLINE UPK_AutoMoveSystem* GetAutoMoveSystem() const { return autoMoveSystem; };
	FORCEINLINE UPK_ShootSystem* GetShootSystem() const { return shootSystem; };
	FORCEINLINE UPK_TimeDilationPower* GetTimeDilationPower() const { return timeDilationPower; };
	FORCEINLINE UPK_GPEUsingSystem* GetGPESystem() const { return GPESystem; };
	FORCEINLINE UPK_GrappleSystem* GetGrappleSystem() const { return grappleSystem; };

	FORCEINLINE USceneComponent* GetCameraSlot() const { return cameraSlot; };
	FORCEINLINE USceneComponent* GetEndCable() const { return endCable; };
	FORCEINLINE APK_Camera* GetCamera() const { return camera; };
	FORCEINLINE UCableComponent* GetCable() const { return cable; };

public:
	FORCEINLINE TArray<AActor*> GetIgnoredActors() const { return ignore; };
	FORCEINLINE float GetBaseMaxSpeed() const { return characterMaxSpeed; };
	FORCEINLINE float GetBaseMaxAcceleration() const { return characterMaxAcceleration; };
	FORCEINLINE float GetBaseGravity() const { return characterGravity; };
	FORCEINLINE float GetBaseHeight() const { return baseHeight; };
	FORCEINLINE float GetBaseFriction() const { return baseFriction; };
	FORCEINLINE float GetBaseBraking() const { return baseBraking; };
	FORCEINLINE float GetBaseBrakingFalling() const { return baseBrakingFalling; };

public:
	void InitMainCharacter();
	void InitHUD();
	void InitAnimations();
	void InitParams();
	void InitDelegates();
	void InitGameMod();
	void CreateCamera();

	void SetIsKeyboard(FKey _key);

	void RefreshAnimations();
	void RefreshIsFalling();

	void RefreshIsFallingGround();

	void RefreshIsFallingWallrun();

	//Player Movements
	void MoveForward(const float _direction);
	void MoveLateral(const float _direction);

	//void Aim();
	//void UnAim();
	void Shoot();
	void TimeDilate();
	void ShootGrapple();

	void JumpAction();
	void Jump();

	UFUNCTION()
	void ResetJumpCount(const FHitResult& _hit);
	void ResetJumpCount();

	void Slide();
	void EndSlide();

	bool CheckSlideOO();
	
	void InteractWithGPE();

	//Player TurnAround
	void LookVertical(const float _direction);
	void LookHorizontal(const float _direction);

	void ActivateAllMovements(const bool _activate);
	void ActivateMove(const bool _activate);
	void ActivateSlide(const bool _activate);
	void ActivateWallrun(const bool _activate);
	void ActivateClimb(const bool _activate);
	void ActivateGrapple(const bool _activate);
};
