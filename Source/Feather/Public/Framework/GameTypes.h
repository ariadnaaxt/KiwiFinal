// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class E_ViewMode : uint8
{
	EVM_ThirdPerson,
	EVM_SideScroller
};

UENUM()
enum class E_CameraState : uint8
{
	ECS_Idle,
	ECS_Moving
};

UENUM()
enum class E_GameState : uint8
{
	EGS_Explore,
	EGS_Pursuit,
	EGS_Cinematic
};

class FEATHER_API GameTypes
{
public:
	GameTypes();
	~GameTypes();
};
