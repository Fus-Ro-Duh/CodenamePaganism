// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.h"
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
	FTimerHandle LoadTimerHandle;                    // ������ ��� ��������� ���� ��������� ������, ���� ������ � ��������� ������ ������
	FTimerHandle RestoringFOVTimerHandle;            // ������ ��� �������������� ���� ������ ����� ��������

	void Load();                                     // ������� ���������
	void Release();                                  // ������� ������� ������

protected:
	virtual void BeginPlay() override;

	// ������ ��� ����������� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	USkeletalMeshComponent* WeaponMesh;

	// ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TArray<FAnimations> Animations;

	// ������������ ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MaxShotPower = 10;

	// ������� ��������� ���� ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float PowerIncreaseRate = 0.1f;

	// ������������ ��������� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MaxCamerashake = 100.0f;

	// ����� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<ABaseProjectile> ArrowProjectile;

	// ����� ������ ������ (������������� ������ � blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UCameraShakeBase> CameraShakeBase;

	// ����������� ������ ��� ������������ (� ��������)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraFOVDelta = 20;

	// ������� �������������� ���� ������ ����� ������������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraFOVRestoreRate = 0.1f;

	// ������������ ������ ��� ������������ ������
	UPROPERTY(EditDefaultsOnly, Category = "Shot")
	FName ArrowSocketName = "ArrowSocketName";

private:
	/* ������� �������� */
	void IncreaseShotPower();                        // �������� �������� ���� ��������
	UFUNCTION()
	void StartIncreasingPower();                     // �������� ���� ��������, ��������� ���� ������ ������ � ��� ���� ���� �������� IncreaseCameraShake()
	void IncreaseCameraShake();                      // �������� ���� ������ ������
	UFUNCTION()
	void SpawnArrow();                               // ������� �� ����� ������ ������ � ��������������
	ABaseProjectile* CurArrowProjectile = nullptr;   // ��������� �� ���������� ������ ������
	UCameraShakeBase* CurCameraShakeBase = nullptr;  // ��������� �� ��������� ����� ������
	float ShotPower = 0.0f;                          // ��������� ���� ���������
	float CameraShake = 0.0f;                        // ��������� ������ ������

	/* ������� ��� ������ */
	void InitAnimations();                           // ������� listener �� notify ��������

	/* ������� ��� �������� */
	bool IsLoadAnimationPlaying = false;             // ������� ���������� ��� �������� �������� �������

	/* ������� ��� ��������� ���� ������ */
	float DefaultCameraFOV = 90.0f;                  // ��������� ���� ������
	float CurrentCameraFOVRate = 0;                  // ����������� ������� ��������� ���� ������
	float MinCameraFOV = DefaultCameraFOV - CameraFOVDelta;
	                                                 // ����������� ���� ������
	float GetCameraFOV();                            // �������� ������� ���� ������
	void RestoreFOV();                               // ��������������� ���� ������ �� ���������� (DefaultCameraFOV)
	void SetFOV(float CurrentCameraFOV);             // ������������� ���� ������
};
