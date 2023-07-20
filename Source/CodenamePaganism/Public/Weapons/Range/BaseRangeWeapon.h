// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseRangeWeapon.generated.h"

class ABaseProjectile;
class UCameraShakeBase;

UCLASS()
class CODENAMEPAGANISM_API ABaseRangeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseRangeWeapon();
	FTimerHandle LoadTimerHandle;
	FTimerHandle RestoringFOVTimerHandle;

	void Load();
	void Release();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* LoadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ReleaseAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MaxShotPower = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float PowerIncreaseRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MaxCamerashake = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<ABaseProjectile> ArrowProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UCameraShakeBase> CameraShakeBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraFOVDelta = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraFOVRestoreRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Shot")
	FName ArrowSocketName = "ArrowSocketName";

private:
	ABaseProjectile* CurArrowProjectile = nullptr;
	UCameraShakeBase* CurCameraShakeBase = nullptr;
	float ShotPower = 0.0f;
	float CameraShake = 1.0f;
	void IncreaseShotPower();
	void IncreaseCameraShake();
	void StartIncreasingPower();
	void InitAnimations();
	void SpawnArrow();
	float DefaultCameraFOV = 90.0f;
	float CurrentCameraFOV = DefaultCameraFOV;
	float MinCameraFOV = DefaultCameraFOV - CameraFOVDelta;
	void GetDefaultCameraFOV();
	void RestoreFOV();
	void SetFOV();
};
