// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CODENAMEPAGANISM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	FTimerHandle RegerateTimerHandle;
	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;

	float GetCurrentHealth() { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const { return FMath::IsNearlyZero(CurrentHealth); }

	bool TryToAddHealth(float HealthAmount);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Regenerate")
	bool AutoRegenerate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regenerate")
	float RegenerateRate = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regenerate")
	float RegenerateLag = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regenerate")
	float RegHealth = 1.0f;

private:
	float CurrentHealth = 0.0f;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void RegenarateHealth();
	void SetHealth(float NewHealth);

	bool IsHealthFull();
};
