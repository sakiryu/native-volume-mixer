#include "pipe_message_handler.h"

PipeMessageHandler::PipeMessageHandler(std::unique_ptr<IVolumeMixer>&& volume_mixer) :
	m_volume_mixer{ std::move(volume_mixer) },
	m_handlers{}
{
	m_handlers["/set_volume"] = std::bind(&PipeMessageHandler::handle_set_volume, this, std::placeholders::_1, std::placeholders::_2);
	m_handlers["/get_volume"] = std::bind(&PipeMessageHandler::handle_get_volume, this, std::placeholders::_1, std::placeholders::_2);
	m_handlers["/close_app"] = std::bind(&PipeMessageHandler::handle_close_app, this, std::placeholders::_1, std::placeholders::_2);
}

void PipeMessageHandler::handle_message(PipeChannel& sender, const std::vector<std::string>& message)
{
	if (message.empty())
	{
		return;
	}

	const auto message_type = message[0];

	auto itHandler = m_handlers.find(message_type);
	if (itHandler == m_handlers.end())
	{
		return;
	}

	auto& [_, handler] = *itHandler;
	handler(sender, message);
}

void PipeMessageHandler::handle_get_volume(PipeChannel& sender, const std::vector<std::string>& message)
{
	const auto volume = m_volume_mixer->get_volume();
	sender.reply(std::to_string(volume));
}

void PipeMessageHandler::handle_set_volume(PipeChannel&, const std::vector<std::string>& message)
{
	if (message.size() < 2)
	{
		return;
	}

	const auto message_arg = message[1];
	const auto volume = std::stoi(message_arg);

	m_volume_mixer->set_volume(volume);
}

void PipeMessageHandler::handle_close_app(PipeChannel& sender, const std::vector<std::string>&)
{
	sender.close();
}