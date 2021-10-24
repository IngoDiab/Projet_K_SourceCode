#include "Debug.h"
#include "DrawDebugHelpers.h"

void Debug::ScreenMessage(const FString& _message, FColor _color)
{
	GEngine->AddOnScreenDebugMessage(-1, -1, _color, _message);
}

void Debug::ScreenMessage(const FString& _message, FColor _color, const float& _time)
{
	GEngine->AddOnScreenDebugMessage(-1, _time, _color, _message);
}

void Debug::DrawSphere(const UWorld* _world, const FVector& _center, const float&_radius, const int32& _segments,
	const FColor&_color, const float& _lifeTime)
{
	DrawDebugSphere(_world, _center, _radius, _segments, _color, false, _lifeTime);
}

void Debug::DrawLine(const UWorld* _world, const FVector& _a, const FVector& _b, const FColor& _color,
	const float& _lifeTime)
{
	DrawDebugLine(_world, _a, _b, _color, false, _lifeTime);
}

void Debug::DrawCube(const UWorld* _world, const FVector& _center, const FVector& _extent, const FColor& _color,
	const float& _lifeTime)
{
	DrawDebugBox(_world,_center,_extent, _color, false, _lifeTime);
	
}
