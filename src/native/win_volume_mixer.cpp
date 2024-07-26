#include "volume_mixer.h"

class WinVolumeMixer : public IVolumeMixer
{
public:
	float get_volume() override
	{
		return .0f;
	}
	
	bool set_volume(float value) override
	{
		return true;
	}
};