// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJET_K_API Debug
{
public:
	static void ScreenMessage(const FString& _message, FColor _color = FColor::White);
	static void ScreenMessage(const FString& _message, FColor _color = FColor::White, const float& _time = -1);
	static void DrawSphere(const UWorld* _world, const FVector& _center, const float& _radius, const int32& _segments,
	                const FColor& _color = FColor::White, const float& _lifeTime = -1);
	static void DrawLine(const UWorld* _world, const FVector& _a, const FVector&_b, const FColor& _color = FColor::White,
		const float& _lifeTime = -1);
	static void DrawCube(const UWorld* _world, const FVector& _center, const FVector& _extent, const FColor& _color = FColor::White,
		const float&_lifeTime = -1);
};
