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

	// Это логистическая функция
	// Она создает красивую S-функцию для плавности
	// Для проверки используй: https://www.mathway.com/ru/Graph и https://en.wikipedia.org/wiki/Logistic_function

	static float GetSFunctionResult(float L , float k, float Med /* Это х0 */, float x, float yDelta = 0 /* нужно для повышения/понижения результата */)
	{
		return L / (1 + FMath::Pow(UE_EULERS_NUMBER, (-k * (x - Med)))) + yDelta;
	}

	static UAnimMontage* GetAnimMontage(TArray<FAnimations> AnimArray, FName NameOfAnimMontage)
	{
		FAnimations* FoundEntry = AnimArray.FindByPredicate([](const FAnimations& InItem)
			{
				return InItem.NameOfAnimation == NameOfAnimMontage;
			});
		if (FoundEntry)
		{
			return FoundEntry->AnimMontage;
		}
		return nullptr;
	}
};