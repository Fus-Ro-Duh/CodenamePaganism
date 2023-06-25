// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class ABaseMeleeWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CODENAMEPAGANISM_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();
	void Attack();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponEquipSocketName = "HandWithSword";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ABaseMeleeWeapon* MeleeWeapon = nullptr;

private:	
	void SpawnWeapons();
	void AttachToWeaponSocket(ABaseMeleeWeapon* Weapon, USceneComponent* SceneComponent, FName& SocketName);
	void EquipWeapon(ABaseMeleeWeapon* CurrentWeapon);
};
