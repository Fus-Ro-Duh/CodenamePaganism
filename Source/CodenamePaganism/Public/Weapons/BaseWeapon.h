// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class CODENAMEPAGANISM_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

	virtual void Attack() {};

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")


protected:
	virtual void BeginPlay() override;

};
