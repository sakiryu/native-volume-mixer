#pragma once

class IVolumeMixer
{
public:
	virtual float get_volume() = 0;
	virtual bool set_volume(float value) = 0;

	virtual ~IVolumeMixer() = default;
};