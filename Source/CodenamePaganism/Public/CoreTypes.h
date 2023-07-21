#pragma once

#include "CoreTypes.generated.h"

class UBaseAnimMontage;

//Health component
//DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);

// ��������� ��� �������� � notify
USTRUCT(BlueprintType)
struct FAnimations
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	FName NameOfAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TArray<FNotifyFunc> AnimNotifies;
};

// ��������� ��� notify - ������� ��� ������ (!!! ������ ���������� ������� �������, �� ��� �� ����� !!!)
USTRUCT(BlueprintType)
struct FNotifyFunc
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TSubclassOf<UBaseAnimMontage> Notify;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	FName FuncName;
};