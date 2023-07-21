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
	FTimerHandle LoadTimerHandle;                    // Таймер для повышения силы натяжения стрелы, угла обзора и повышения тряски камеры
	FTimerHandle RestoringFOVTimerHandle;            // Таймер для восстановления угла обзора после выстрела

	void Load();                                     // Функция натяжения
	void Release();                                  // Функция выпуска стрелы

protected:
	virtual void BeginPlay() override;

	// Данные для отображения оружия
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	USkeletalMeshComponent* WeaponMesh;

	// Анимации
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TArray<FAnimations> Animations;

	// Максимальная сила заряда
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MaxShotPower = 10;

	// Частота повышения силы стрельбы
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float PowerIncreaseRate = 0.1f;

	// Максимальная амплитуда тряски
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MaxCamerashake = 100.0f;

	// Класс стрелы
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<ABaseProjectile> ArrowProjectile;

	// Класс тряски камеры (настраивается только в blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UCameraShakeBase> CameraShakeBase;

	// Приближение камеры при прицеливании (в градусах)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraFOVDelta = 20;

	// Частота восстановления угла обзора после прицеливания
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraFOVRestoreRate = 0.1f;

	// Наименование сокета для прикрепления стрелы
	UPROPERTY(EditDefaultsOnly, Category = "Shot")
	FName ArrowSocketName = "ArrowSocketName";

private:
	/* Функции стрельбы */
	void IncreaseShotPower();                        // Начинает набирать силу выстрела
	UFUNCTION()
	void StartIncreasingPower();                     // Повышает силу выстрела, уменьшает угол обзора камеры и при макс силе вызывает IncreaseCameraShake()
	void IncreaseCameraShake();                      // Повышает силу тряски камеры
	UFUNCTION()
	void SpawnArrow();                               // Создает на сцене актера стрелы с трансформацией
	ABaseProjectile* CurArrowProjectile = nullptr;   // Указатель на созданного актера стрелы
	UCameraShakeBase* CurCameraShakeBase = nullptr;  // Указатель на созданный класс тряски
	float ShotPower = 0.0f;                          // Начальная сила натяжения
	float CameraShake = 0.0f;                        // Начальная тряска камеры

	/* Функции для старта */
	void InitAnimations();                           // Создает listener на notify анимаций

	/* Функции для выстрела */
	bool IsLoadAnimationPlaying = false;             // Булевая переменная для проверки анимации зарядки

	/* Функции для изменения угла обзора */
	float DefaultCameraFOV = 90.0f;                  // Начальный угол обзора
	float CurrentCameraFOVRate = 0;                  // Накопленная частота изменения угла обзора
	float MinCameraFOV = DefaultCameraFOV - CameraFOVDelta;
	                                                 // Минимальный угол обзора
	float GetCameraFOV();                            // Получает текущий угол обзора
	void RestoreFOV();                               // Восстанавливает угол обзора до начального (DefaultCameraFOV)
	void SetFOV(float CurrentCameraFOV);             // Устанавливает угол обзора
};
