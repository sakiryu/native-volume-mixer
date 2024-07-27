#include "volume_mixer.h"

#include <stdexcept>

#include <windows.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <atlbase.h>

// https://learn.microsoft.com/en-us/windows/win32/coreaudio/volume-controls

class WinVolumeMixer : public IVolumeMixer
{
	CComPtr<IMMDeviceEnumerator> m_device_enumerator;
	CComPtr<IMMDevice> m_audio_device;
	CComPtr<IAudioEndpointVolume> m_audio_endpoint;

	void initialize()
	{
		if (auto status = CoInitialize(nullptr); FAILED(status))
		{
			throw std::runtime_error("Failed to initialize COM.");
		}

		if (auto status = m_device_enumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER); FAILED(status))
		{
			throw std::runtime_error("Failed to enumerate audio devices.");
		}

		if (auto status = m_device_enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_audio_device); FAILED(status))
		{
			throw std::runtime_error("Failed to identify the default audio device.");
		}

		if (auto status = m_audio_device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, nullptr, (void**)&m_audio_endpoint); FAILED(status))
		{
			throw std::runtime_error("Failed to activate audio endpoint.");
		}
	}

public:
	WinVolumeMixer() :
		m_device_enumerator{},
		m_audio_device{},
		m_audio_endpoint{}
	{
		initialize();
	}

	float get_volume() override
	{
		float volume_level{ .0f };
		m_audio_endpoint->GetMasterVolumeLevelScalar(&volume_level);
		return volume_level;
	}
	
	bool set_volume(float value) override
	{
		return true;
	}

	~WinVolumeMixer()
	{
		CoUninitialize();
	}
};