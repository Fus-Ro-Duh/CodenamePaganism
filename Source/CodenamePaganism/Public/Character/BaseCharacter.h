// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class UInputMappingContext;
class UInputAction;
class UHealthComponent;
class UWeaponComponent;

UCLASS()
class CODENAMEPAGANISM_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjInit);
	virtual void OnDeath();

	bool IsWalking = false;
	bool IsRunning = false;

protected:
	virtual void BeginPlay() override;

	//Fall damage properties
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CameraCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWeaponComponent* WeaponComponent;

	//Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputMappingContext* OnFootMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* IAMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* IALook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* IAJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* IACrouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* IAWalk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* IARun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* IAAttack;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Movement Functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpChar(const FInputActionValue& Value);
	void CrouchChar(const FInputActionValue& Value);
	void UnCrouchChar(const FInputActionValue& Value);
	void Walk(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void StopRun(const FInputActionValue& Value);
	void Load(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

	//Camera collision functions
	void CheckCameraOverlap();
	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Setting up input
	void SetupPlayerInput();

	//Fall damage
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);
};
