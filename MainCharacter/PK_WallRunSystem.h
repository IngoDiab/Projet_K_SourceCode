// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Projet_K/Utils/Action/Action.h"
#include "PK_WallRunSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJET_K_API UPK_WallRunSystem : public UActorComponent
{
	GENERATED_BODY()

	#pragma region Owner
	UPROPERTY()
		class APK_MainCharacter* owner = nullptr;

	UPROPERTY()
		class UPK_MovementBehaviour* ownerMovementBehaviour = nullptr;

	UPROPERTY()
		class UCharacterMovementComponent* ownerMovement = nullptr;
	
	UPROPERTY()
		class UPK_GrappleSystem* ownerGrappleSystem = nullptr;

	UPROPERTY()
		class UPK_ClimbSystem* ownerClimbSystem = nullptr;
	
	UPROPERTY()
		class UCapsuleComponent* capsulePlayer = nullptr;
	
	UPROPERTY()
		class APK_Camera* camera = nullptr;
	#pragma endregion

protected:

	UPROPERTY(EditAnywhere, Category = "Wallrun", meta = (DisplayName = "Has Initial Impulse"))
		bool hasImpulse = true;

	UPROPERTY(VisibleAnywhere, Category = "Wallrun", meta = (DisplayName = "In Initial Impulse"))
		bool isInImpulse = false;

	UPROPERTY(EditAnywhere, Category = "Wallrun", meta = (DisplayName = "Initial Impulse"))
		float impulse = 250;

	UPROPERTY(EditAnywhere, Category = "Wallrun", meta = (DisplayName = "Timer Max Initial Impulse"))
		float timerMaxImpulse = .5f;

	UPROPERTY()
		float timerImpulse = 0;

	UPROPERTY()
		FVector impulseWallRun = FVector::UpVector;

	#pragma region BooleanWallrun
	UPROPERTY(EditAnywhere, Category = "Wallrun", meta = (DisplayName = "Player can wallrun"))
		bool canWallRun = true;
	
	UPROPERTY(VisibleAnywhere, Category = "Wallrun", meta = (DisplayName = "Player can again wallrun"))
		bool canWallRunAgain = true;

	UPROPERTY(VisibleAnywhere, Category = "Wallrun", meta = (DisplayName = "Player is wallrunning"), BlueprintReadWrite)
		bool isWallRunning = false;

	UPROPERTY(VisibleAnywhere, Category = "Wallrun", meta = (DisplayName = "Player is about to wallrun"))
		bool isAboutToWallrun = false;

	UPROPERTY(VisibleAnywhere, Category = "Wallrun", meta = (DisplayName = "Player was wallrunning"))
		bool wasWallrunning = false;

	//
	UPROPERTY(VisibleAnywhere, Category = "Wallrun", meta = (DisplayName = "Player was wallrunning left wall"))
		bool wasWallrunningOnLeftWall = false;

	UPROPERTY(VisibleAnywhere, Category = "Wallrun", meta = (DisplayName = "Player was wallrunning right wall"))
		bool wasWallrunningOnRightWall = false;

	//

	UPROPERTY(VisibleAnywhere, Category = "Wallrun", meta = (DisplayName = "Player is wallrunning on a left wall"))
		bool isWallRunningOnLeftWall = false;

	UPROPERTY(VisibleAnywhere, Category = "Wallrun", meta = (DisplayName = "Player is wallrunning on a right wall"))
		bool isWallRunningOnRightWall = false;
	#pragma endregion

	UPROPERTY()
		float targetedRoll = 0;

	UPROPERTY(EditAnywhere, Category = "Wallrun", meta = (DisplayName = "Angle between player and wall while wallrunning"))
		float angleWallRun = 15;
	
	UPROPERTY(EditAnywhere, Category = "Wallrun", meta = (DisplayName = "Player rotate speed"))
		float speedRotation = 100;
	
	UPROPERTY(EditAnywhere, Category = "Wallrun", meta = (DisplayName = "Wallrun speed"))
		float wallrunSpeed = 1250;
	
	UPROPERTY(EditAnywhere, Category = "Wallrun", meta = (DisplayName = "Wallrun acceleration"))
		float wallrunAcceleration = 2048;

	UPROPERTY(EditAnywhere, Category = "Wallrun", meta = (DisplayName = "Tag a wall must have to be wallrunnable"))
		FName TagWallRun = "WallRun";


	UPROPERTY()
		FVector directionWallRun = FVector::ZeroVector;
	
	UPROPERTY()
		FVector currentNormalWallRun = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Wallrun | Constraints | Stop", meta = (DisplayName = "Player fall when stop on wall"))
		bool fallOnStop = false;

	UPROPERTY(EditAnywhere, Category = "Wallrun | Constraints | Stop", meta = (DisplayName = "Minimal speed to consider player stopped"))
		float minSpeedWallRun = 150;

	UPROPERTY(EditAnywhere, Category = "Wallrun | Constraints | Gravity", meta = (DisplayName = "Activate Gravity"))
		bool activateGravity = false;

	UPROPERTY(EditAnywhere, Category = "Wallrun | Constraints | Gravity", meta = (DisplayName = "Gravity"))
		float gravity = 15;

	UPROPERTY(EditAnywhere, Category = "Wallrun | Constraints | Angle", meta = (DisplayName = "Player fall when look angle is too wide"))
		bool fallAngleToWide = false;
	
	UPROPERTY(EditAnywhere, Category = "Wallrun | Constraints | Angle", meta = (DisplayName = "Max angle"))
		float angleToWide = 70;

	UPROPERTY(EditAnywhere, Category = "Wallrun | Constraints | Angle", meta = (DisplayName = "Speed clamp angle on wallrun"))
		float speedClampAngle = 500;

	UPROPERTY(EditAnywhere, Category = "WallJump | Casual Jump", meta = (DisplayName = "Jump on X"))
		float jumpForward = 1750;

	UPROPERTY(EditAnywhere, Category = "WallJump | Casual Jump", meta = (DisplayName = "Jump on Z"))
		float jumpUp = 300;

	UPROPERTY(EditAnywhere, Category = "WallJump | Forward Jump", meta = (DisplayName = "Min Angle"))
		float minAngleJump = 15;

	UPROPERTY(EditAnywhere, Category = "WallJump | Forward Jump", meta = (DisplayName = "Jump on X"))
		float jumpOnX = 0;

	UPROPERTY(EditAnywhere, Category = "WallJump | Forward Jump", meta = (DisplayName = "Jump on Y"))
		float jumpOnY = 750;

	UPROPERTY(EditAnywhere, Category = "WallJump | Forward Jump", meta = (DisplayName = "Use Jump on Z"))
		bool useJumpOnZ = true;

	UPROPERTY(EditAnywhere, Category = "WallJump | Forward Jump", meta = (DisplayName = "Jump on Z"))
		float jumpOnZ = 1000;
	
	UPROPERTY(EditAnywhere, Category = "WallJump | Forward Jump", meta = (DisplayName = "(Camera) Jump on Z"))
		float jumpOnZCamera = 1450;

	UPROPERTY(EditAnywhere, Category = "WallJump | Forward Jump", meta = (DisplayName = "Is Clamp minimum Z"))
		bool isMinClampZJump = true;

	UPROPERTY(EditAnywhere, Category = "WallJump | Forward Jump", meta = (DisplayName = "Clamp minimum Z"))
		float minClampZJump = 0;

	UPROPERTY(EditAnywhere, Category = "WallJump | Forward Jump", meta = (DisplayName = "Is Clamp maximum Z"))
		bool isMaxClampZJump = false;

	UPROPERTY(EditAnywhere, Category = "WallJump | Forward Jump", meta = (DisplayName = "Clamp maximum Z"))
		float maxClampZJump = 1000;

	UPROPERTY(VisibleAnywhere, Category = "Floor Detection", meta = (DisplayName = "Player is on floor"))
		bool isOnFloor = false;

	UPROPERTY(EditAnywhere, Category = "Floor Detection", meta = (DisplayName = "Minimale height to be on floor"))
		float minDistanceFloor = 150;

	UPROPERTY()
		class UCapsuleComponent* hitboxWallrun = nullptr;


	DECLARE_EVENT(UPK_WallRunSystem, CheckConditions)
	CheckConditions onWallrunChecks;

	DECLARE_EVENT_OneParam(UPK_WallRunSystem, WallrunRotations, float)
	WallrunRotations onWallUpdateRotation;
	
	DECLARE_EVENT(UPK_WallRunSystem, EndWallrun)
	EndWallrun onWallrunEnd;

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeginWallrun);
	UPROPERTY(BlueprintAssignable)
		FBeginWallrun onWallrunBegin;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWalljump);
	UPROPERTY(BlueprintAssignable)
		FWalljump onWalljump;

public:	
	// Sets default values for this component's properties
	UPK_WallRunSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
		void ExitTriggerHitbox();

public:
	FORCEINLINE void SetCanWallRun(const bool _canWallRun) { canWallRun = _canWallRun; };
	FORCEINLINE bool IsWallRuning() const { return isWallRunning; };
	FORCEINLINE bool GetIsWallRuning() { return isWallRunning; };
	FORCEINLINE void SetIsWallRuning(const bool _isWallruning) { isWallRunning = _isWallruning; };

public:
	FORCEINLINE FBeginWallrun* OnWallrunBegin() { return &onWallrunBegin; };
	FORCEINLINE CheckConditions* OnWallrunChecks() { return &onWallrunChecks; };
	FORCEINLINE WallrunRotations* OnWallUpdateRotation() { return &onWallUpdateRotation; };
	FORCEINLINE EndWallrun* OnWallrunEnd() { return &onWallrunEnd; };
	FORCEINLINE FWalljump* OnWalljump() { return &onWalljump; };

public:

	//Init Component
	void InitOwnerMovement();

	void GetCamera();

	void InitOwnerDelegate();

	void InitOwnerComponents();

	void InitDelegate();

	//Constraints
	void SetConstraints();
	void ResetConstraints();

	//Register Direction
	void WallDetectionSystem();
	bool StayOnWall(float _way);
	bool DetectWall(float _way);
	bool CollectionsLineTrace(const float _way, const float _angleBack, const float _angleForward, const float _length);
	bool LineTraceWallrun(const float _way, const float _offsetForward, const float _length);

	bool LineTraceWallrun(const float _way, const float _offsetForward, const float _length, FHitResult& _hit);
	
	//Wallrun
	void MoveOnWall(const float _direction);
	void BeginWallrun();

	UFUNCTION()
	void Wallrun();
	void QuitWallRun();
	void ResetOnQuitWallRun();
	void ResetWallrun();
	void ResetWallrunOnCancel();
	void ResetWallrunAfterWallrun();
	void ResetInitialImpulseTimer();

	//Walljump
	void WallJump();
	void WallJumpTooFront();
	FVector CalculateUp();
	FVector CalculateUpWithZ();

	//Checks
	void CheckForWallToRunOn();
	void CheckResetWallrun();
	void CheckAngleOnWall();
	void CheckVelocityOnWall();
	void CheckStayOnWall();
	void CheckOnFloor();
	void CheckInitialImpulseTimer(const float _tick);

	//ViewConstraints
	void ClampView(const float _tick);
	bool AllowRotation();
	void RotateToTargeted(const float _tick);
	void ResetRotation();
	void SetTargetedRotation(const float _offset);
};
