// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMeleeWeapon.generated.h"

UCLASS()
class CODENAMEPAGANISM_API ABaseMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseMeleeWeapon();


protected:
	virtual void BeginPlay() override;

public:


};
