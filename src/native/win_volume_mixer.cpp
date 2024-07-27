#include "volume_mixer.h"

#include <stdexcept>

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

float VolumeMixer::get_volume()
{
	float volume_level{ .0f };
	m_volume_mixer_impl->AudioEndpoint->GetMasterVolumeLevelScalar(&volume_level);
	return volume_level;
}

bool VolumeMixer::set_volume(float volume_level)
{
	if (auto status = m_volume_mixer_impl->AudioEndpoint->SetMasterVolumeLevelScalar(volume_level, nullptr); FAILED(status))
	{
		return false;
	}
	return true;
}