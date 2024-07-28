#pragma once

#include "pipe_channel.h"
#include "pipe_message_handler.h"

class App
{
	PipeMessageHandler m_message_handler;
	PipeChannel m_channel;

public:
	App();

	void run();
};

