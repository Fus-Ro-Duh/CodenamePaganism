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
#include "Utils.h"

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
		if (UAnimMontage* AnimMontage = Utils::GetAnimMontage(Animations, "LoadAnimation"))
		{
			Character->PlayAnimMontage(AnimMontage);
			IsLoadAnimationPlaying = true;
		}
	}
}

void ABaseRangeWeapon::Release()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(LoadTimerHandle);

		if (CurArrowProjectile)
		{
			if (IsLoadAnimationPlaying || !ShotPower)       // Ïðîâåðêà íà èãðàþùóþ àíèìàöèþ è íóëåâóþ ñèëó ñòðåëüáû
			{                                               // Ïîêà ÷òî ñòîèò óäàëåíèÿ ñòðåëû ñî ñöåíû
				CurArrowProjectile->Destroy();
				if (const auto Character = Cast<ACharacter>(GetOwner())) Character->StopAnimMontage(); // Îòêëþ÷àåì àíèìàöèþ
				return;
			}

			CurArrowProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);  // Îòêðåïëÿåò ñòðåëó îò ñîêåòà
			CurArrowProjectile->ShootArrow(ShotPower);                                           // Âûçûâàåò ôóíêöèþ ñòðåëüáû
			if (const auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
			{
				CameraManager->StopCameraShake(CurCameraShakeBase, true);
			}

			if (const auto Character = Cast<ACharacter>(GetOwner()))
			{
				if (UAnimMontage* AnimMontage = Utils::GetAnimMontage(Animations, "ReleaseAnimation"))
		{
			Character->PlayAnimMontage(AnimMontage);
		}
			}
		}

		ShotPower = 0.0f;
		CameraShake = 0.0f;
		GetWorldTimerManager().SetTimer(RestoringFOVTimerHandle, this, &ABaseRangeWeapon::RestoreFOV, CameraFOVRestoreRate, true); // Âîññòàíàâëèâàåì óãîë îáçîðà êàìåðû
	}
}

void ABaseRangeWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	InitAnimations();
	DefaultCameraFOV = GetCameraFOV();
}

void ABaseRangeWeapon::IncreaseShotPower()
{
	ShotPower = FMath::Clamp(ShotPower+1, 0.0f, MaxShotPower);
	CurrentCameraFOVRate = CurrentCameraFOVRate + PowerIncreaseRate;
	SetFOV(Utils::GetSFunctionResult(CameraFOVDelta, -0.5, CameraFOVDelta / 2, CurrentCameraFOVRate, MinCameraFOV));
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
		ArrowSpawnNotify->OnNotified.AddUFunction(this, CallFunc);
	}
	auto LoadNotify = AnimUtils::FindNotifyByClass<ULoadAnimNotify>(LoadAnimation);
	if (LoadNotify)
	{
		LoadNotify->OnNotified.AddUObject(this, &ABaseRangeWeapon::StartIncreasingPower);
	}
	if(!Notify)
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
	IsLoadAnimationPlaying = false;
}

float ABaseRangeWeapon::GetCameraFOV()
{
	if(APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		return CameraManager->GetFOVAngle();
	}
	return 0.f;
}

void ABaseRangeWeapon::RestoreFOV()
{
	CurrentCameraFOVRate = CurrentCameraFOVRate - CameraFOVRestoreRate;
	SetFOV(Utils::GetSFunctionResult(CameraFOVDelta, -0.5, CameraFOVDelta/2, CurrentCameraFOVRate, MinCameraFOV));
	if (FMath::IsNearlyEqual(GetCameraFOV(), DefaultCameraFOV))
	{
		GetWorldTimerManager().ClearTimer(RestoringFOVTimerHandle);
		CurrentCameraFOVRate = 0;
	}
}

void ABaseRangeWeapon::SetFOV(float CurrentCameraFOV)
{
	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		CameraManager->SetFOV(CurrentCameraFOV);
	}
}