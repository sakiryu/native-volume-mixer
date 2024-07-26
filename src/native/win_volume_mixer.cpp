#include "volume_mixer.h"

#include <stdexcept>

#include <windows.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>

// https://learn.microsoft.com/en-us/windows/win32/coreaudio/volume-controls

class WinVolumeMixer : public IVolumeMixer
{
	IMMDeviceEnumerator* m_device_enumerator;
	IMMDevice* m_audio_device;

	bool init_device_enumerator()
	{
		const auto status_result = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_device_enumerator));
		if (FAILED(status_result))
		{
			return false;
		}
		return true;
	}

	bool init_audio_device()
	{
		const auto status_result = m_device_enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_audio_device);
		if (FAILED(status_result))
		{
			m_device_enumerator->Release();
			return false;
		}
		return true;
	}

public:
	WinVolumeMixer() : m_device_enumerator{ nullptr }, m_audio_device{nullptr}
	{
		const auto status_result = CoInitialize(nullptr);
		if (FAILED(status_result))
		{
			throw std::runtime_error("Failed to initialize COM.");
		}

		init_device_enumerator();
		init_audio_device();
	}

	float get_volume() override
	{
		return .0f;
	}
	
	bool set_volume(float value) override
	{
		return true;
	}

	~WinVolumeMixer()
	{
		m_audio_device->Release();
		m_device_enumerator->Release();
		CoUninitialize();
	}
};