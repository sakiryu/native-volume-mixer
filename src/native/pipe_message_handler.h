#pragma once

#include "volume_mixer.h"
#include "pipe_channel.h"

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

using MessageHandlerCallback = std::function<void(PipeChannel&, const std::vector<std::string>&)>;

class PipeMessageHandler
{
	std::unique_ptr<IVolumeMixer> m_volume_mixer;
	std::unordered_map<std::string, MessageHandlerCallback> m_handlers;

	void handle_get_volume(PipeChannel& sender, const std::vector<std::string>& message);
	void handle_set_volume(PipeChannel& sender, const std::vector<std::string>& message);
	void handle_close_app(PipeChannel& sender, const std::vector<std::string>& message);

public:
	PipeMessageHandler(std::unique_ptr<IVolumeMixer>&& volume_mixer);

	void handle_message(PipeChannel& sender, const std::vector<std::string>& message);
};

