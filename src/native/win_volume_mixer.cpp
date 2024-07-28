#include "volume_mixer.h"

#include <stdexcept>
#include <cmath>

#include <windows.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <atlbase.h>

// https://learn.microsoft.com/en-us/windows/win32/coreaudio/volume-controls
struct NativeVolumeMixerImpl
{
	CComPtr<IAudioEndpointVolume> AudioEndpoint{};

	NativeVolumeMixerImpl()
	{
		if (auto status = CoInitialize(nullptr); FAILED(status))
		{
			throw std::runtime_error("Failed to initialize COM.");
		}

		CComPtr<IMMDeviceEnumerator> device_enumerator{};
		if (auto status = device_enumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER); FAILED(status))
		{
			throw std::runtime_error("Failed to enumerate audio devices.");
		}

		CComPtr<IMMDevice> audio_device{};
		if (auto status = device_enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audio_device); FAILED(status))
		{
			throw std::runtime_error("Failed to identify the default audio device.");
		}

		if (auto status = audio_device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, nullptr, (void**)&AudioEndpoint); FAILED(status))
		{
			throw std::runtime_error("Failed to activate audio endpoint.");
		}
	}

	~NativeVolumeMixerImpl()
	{
		CoUninitialize();
	}
};

VolumeMixer::VolumeMixer() : m_volume_mixer_impl{ std::make_unique<NativeVolumeMixerImpl>() }{}

int VolumeMixer::get_volume()
{
	float volume_level{ .0f };
	m_volume_mixer_impl->AudioEndpoint->GetMasterVolumeLevelScalar(&volume_level);
	return std::lround(volume_level * 100.f);
}

bool VolumeMixer::set_volume(int volume_level)
{
	const auto level{ volume_level * 0.01f };
	if (auto status = m_volume_mixer_impl->AudioEndpoint->SetMasterVolumeLevelScalar(level, nullptr); FAILED(status))
	{
		return false;
	}
	return true;
}