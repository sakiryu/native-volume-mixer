#pragma once

class IVolumeMixer
{
public:
	virtual int get_volume() = 0;
	virtual bool set_volume(int volume_level) = 0;

	virtual ~IVolumeMixer() = default;
};