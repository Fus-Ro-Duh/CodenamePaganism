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
#include "Animations/LoadAnimNotify.h"
#include "Engine/EngineTypes.h"
#include "Weapons/Melee/BaseMeleeWeapon.h"

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

		if (CurArrowProjectile)
		{
			if (!ShotPower) {
				CurArrowProjectile->Destroy();
				return;
			}
			CurArrowProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurArrowProjectile->ShootArrow(ShotPower);
			if (const auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
			{
				CameraManager->StopCameraShake(CurCameraShakeBase, true);
			}

			if (const auto Character = Cast<ACharacter>(GetOwner()))
			{
				Character->PlayAnimMontage(ReleaseAnimation);
			}
		}
		ShotPower = 0.0f;
		CameraShake = 0.0f;
		GetWorldTimerManager().SetTimer(RestoringFOVTimerHandle, this, &ABaseRangeWeapon::RestoreFOV, CameraFOVRestoreRate, true);
	}

}

void ABaseRangeWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	InitAnimations();
	GetDefaultCameraFOV();
}

void ABaseRangeWeapon::IncreaseShotPower()
{
	ShotPower = FMath::Clamp(ShotPower+1, 0.0f, MaxShotPower);
	CurrentCameraFOV = FMath::Clamp(CurrentCameraFOV - 1, MinCameraFOV, DefaultCameraFOV);
	SetFOV();
	if (FMath::IsNearlyEqual(ShotPower,MaxShotPower))
	{
		IncreaseCameraShake();
	}
}

void ABaseRangeWeapon::IncreaseCameraShake()
{
	CameraShake = FMath::Clamp(CameraShake+1, 0.0f, MaxShotPower);
	if (const auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		if (CameraManager) {
			CurCameraShakeBase = CameraManager->StartCameraShake(
				CameraShakeBase,
				CameraShake
			);
		}
	}

}

void ABaseRangeWeapon::StartIncreasingPower()
{
	GetWorldTimerManager().SetTimer(LoadTimerHandle, this, &ABaseRangeWeapon::IncreaseShotPower, PowerIncreaseRate, true);
}

void ABaseRangeWeapon::InitAnimations()
{
	auto ArrowSpawnNotify = AnimUtils::FindNotifyByClass<UArrowSpawnAnimNotify>(LoadAnimation);
	if (ArrowSpawnNotify)
	{
		ArrowSpawnNotify->OnNotified.AddUObject(this, &ABaseRangeWeapon::SpawnArrow);
	}
	auto LoadNotify = AnimUtils::FindNotifyByClass<ULoadAnimNotify>(LoadAnimation);
	if (LoadNotify)
	{
		LoadNotify->OnNotified.AddUObject(this, &ABaseRangeWeapon::StartIncreasingPower);
	}
	if(!ArrowSpawnNotify || !LoadNotify)
	{
		checkNoEntry();
	}
}

void ABaseRangeWeapon::SpawnArrow()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;
	
	CurArrowProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ArrowProjectile);
	if (!CurArrowProjectile) return;

	CurArrowProjectile->SetOwner(Character);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

	CurArrowProjectile->AttachToComponent(Character->GetMesh(), AttachmentRules, ArrowSocketName);
}

void ABaseRangeWeapon::GetDefaultCameraFOV()
{
	if(APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		DefaultCameraFOV = CameraManager->GetFOVAngle();
		CurrentCameraFOV = DefaultCameraFOV;
	}
}

void ABaseRangeWeapon::RestoreFOV()
{
	CurrentCameraFOV= CurrentCameraFOV+0.1f;
	SetFOV();
	if (FMath::IsNearlyEqual(CurrentCameraFOV, DefaultCameraFOV))
	{
		GetWorldTimerManager().ClearTimer(RestoringFOVTimerHandle);
	}
}

void ABaseRangeWeapon::SetFOV()
{
	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		CameraManager->SetFOV(CurrentCameraFOV);
	}
}