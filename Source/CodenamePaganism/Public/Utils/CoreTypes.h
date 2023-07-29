#pragma once

#include "CoreTypes.generated.h"

class UBaseAnimNotify;

//Health component
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

// ��������� ��� notify - ������� ��� ������ (!!! ������ ���������� ������� �������, �� ��� �� ����� !!!)
USTRUCT(BlueprintType)
struct FNotifyFunc
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TSubclassOf<UBaseAnimNotify> Notify;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	FName FuncName;
};

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