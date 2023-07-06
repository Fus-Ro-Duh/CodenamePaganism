// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Range/BaseRangeWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"
#include "Weapons/Range/Projectile/BaseProjectile.h"
#include "Animations/AnimUtils.h"
#include "Animations/ArrowSpawnAnimNotify.h"
#include "Engine/EngineTypes.h"

ABaseRangeWeapon::ABaseRangeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateAbstractDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ABaseRangeWeapon::Load()
{
	if (const auto Character = Cast<ACharacter>(GetOwner()))
	{
		Character->PlayAnimMontage(LoadAnimation);
	}
}

void ABaseRangeWeapon::Release()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(LoadTimerHandle);
	}

	ShotPower = 0.0f;
	CameraShake = 1.0f;

	if (CurArrowProjectile)
	{
		CurArrowProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurArrowProjectile->ShootArrow(ShotPower);
	}
}

void ABaseRangeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	check(WeaponMesh);
}

void ABaseRangeWeapon::IncreaseShotPower()
{
	ShotPower = FMath::Clamp(ShotPower++, 0.0f, MaxShotPower);
	if (FMath::IsNearlyEqual(ShotPower,MaxShotPower))
	{
		IncreaseCameraShake();
	}
}

void ABaseRangeWeapon::IncreaseCameraShake()
{
	CameraShake = FMath::Clamp(CameraShake++, 0.0f, MaxShotPower);
	if (const auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		if (CameraManager) {
			CameraManager->PlayWorldCameraShake(
				GetWorld(),
				CameraShakeBase,
				GetOwner()->GetActorLocation(),
				0.0f,
				50.0f,
				1,
				false
			);
		}
	}

}

void ABaseRangeWeapon::StartIncreasingPower()
{
	GetWorld()->GetTimerManager().SetTimer(LoadTimerHandle, this, &ABaseRangeWeapon::IncreaseShotPower, PowerIncreaseRate, true);
}

void ABaseRangeWeapon::InitAnimations()
{
	auto ArrowSpawnNotify = AnimUtils::FindNotifyByClass<UArrowSpawnAnimNotify>(LoadAnimation);
	if (ArrowSpawnNotify)
	{
		ArrowSpawnNotify->OnNotified.AddUObject(this, &ABaseRangeWeapon::SpawnArrow);
	}
	else if (true)
	{

	}
	else
	{
		checkNoEntry();
	}
}

void ABaseRangeWeapon::SpawnArrow()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	CurArrowProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ArrowProjectile);

	CurArrowProjectile->SetOwner(Character);

	if (!CurArrowProjectile) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurArrowProjectile->AttachToComponent(Character->GetMesh(), AttachmentRules, ArrowSocketName);
}
