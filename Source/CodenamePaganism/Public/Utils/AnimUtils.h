#pragma once

#include "Animations/BaseAnimNotify.h"

class AnimUtils
{
public:
	static UBaseAnimNotify* FindNotifyByClass(UAnimMontage* Animation, TSubclassOf<UBaseAnimNotify> NotifyClass)
	{
		if (Animation)
		{
			for (auto NotifyEvent : Animation->Notifies)
			{
				UBaseAnimNotify* AnimationNotify = Cast<UBaseAnimNotify>(NotifyEvent.Notify);
				if (AnimationNotify && AnimationNotify->IsA(*NotifyClass))
				{
					return AnimationNotify;
				}
			}
		}
		return nullptr;
	}
};