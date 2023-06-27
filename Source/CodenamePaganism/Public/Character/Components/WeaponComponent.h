// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class UAnimMontage;
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
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
<<<<<<< HEAD
	TSubclassOf<ABaseMeleeWeapon> MeleeWeapon;


=======
	ABaseMeleeWeapon* MeleeWeapon;
>>>>>>> 5dab38fca0db9acc9c356f809a3c2e4a39db9671

private:	
	UPROPERTY()
		ABaseMeleeWeapon* CurrentWeapon = nullptr;

	void SpawnWeapons();
	void AttachToWeaponSocket(ABaseMeleeWeapon* Weapon, USceneComponent* SceneComponent, FName& SocketName);
	void EquipWeapon(ABaseMeleeWeapon* CurrentWeapon);

};
