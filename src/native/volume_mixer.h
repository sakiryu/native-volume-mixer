#pragma once

#include "ivolume_mixer.h"

#include <memory>

class VolumeMixer : public IVolumeMixer
{
	std::unique_ptr<struct NativeVolumeMixerImpl> m_volume_mixer_impl;

public:
	VolumeMixer();

	int get_volume() override;
	bool set_volume(int volume_level) override;
};