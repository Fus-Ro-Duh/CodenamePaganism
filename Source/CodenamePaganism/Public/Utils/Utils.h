#pragma once

class Utils
{
public:
	template<typename T>
	static T* GetPlayerComponent(AActor* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}

	// ��� ������������� �������
	// ��� ������� �������� S-������� ��� ���������
	// ��� �������� ���������: https://www.mathway.com/ru/Graph � https://en.wikipedia.org/wiki/Logistic_function

	static float GetSFunctionResult(float L , float k, float Med /* ��� �0 */, float x, float yDelta = 0 /* ����� ��� ���������/��������� ���������� */)
	{
		return L / (1 + FMath::Pow(UE_EULERS_NUMBER, (-k * (x - Med)))) + yDelta;
	}

	static UAnimMontage* GetAnimMontage(TArray<FAnimations> AnimArray, FString NameOfAnimMontage)
	{
		FAnimations* FoundEntry = AnimArray.FindByPredicate([ NameOfAnimMontage ](const FAnimations& InItem)
			{
				return InItem.NameOfAnimation.ToString() == NameOfAnimMontage;
			});
		if (FoundEntry)
		{
			return FoundEntry->AnimMontage;
		}
		return nullptr;
	}
};