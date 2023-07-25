#pragma once

class AnimUtils
{
public:
	template<typename T>
	static T* FindNotifyByClass(UAnimMontage* Animation)
	{
		if (Animation)
		{
			for (auto NotifyEvent : Animation->Notifies)
			{
				if (auto  AnimationNotify = Cast<T>(NotifyEvent.Notify))
				{
					return AnimationNotify;
				}
			}
		}
		return nullptr;
	}
};