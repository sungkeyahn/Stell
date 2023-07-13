// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	None=0, Sword, Hammer, Axe, Spear, Shield, Dagger, Knuckle, Mace
};
UENUM(BlueprintType)
enum class EHitEffectType :uint8
{
	None = 0, Stiff,KnockBack,Airborne, KnockDown
};
UENUM(BlueprintType)
enum class EAttackShapeType :uint8
{
	Line,
	Box,
	Sphere,
	Capsule,
	Weapon
};
UENUM(BlueprintType)
enum class EEnemyState :uint8
{
	Idle, SuperArmor, Groggy, Invincibility, Dead
};