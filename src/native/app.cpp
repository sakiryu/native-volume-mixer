#include "app.h"
#include "volume_mixer.h"

App::App() :
	m_message_handler{ std::make_unique<VolumeMixer>() },
	m_channel{ "VolumeMixer" }
{
	m_channel.on_message_received([&](PipeChannel& sender, const auto& message)
	{
		m_message_handler.handle_message(sender, message);
	});
}

void App::run()
{
	m_channel.open();
}