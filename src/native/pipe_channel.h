#pragma once

#include <string>
#include <vector>
#include <functional>

using MessageReceivedCallback = std::function<void(class PipeChannel&, const std::vector<std::string>&) > ;

class PipeChannel
{
    void* m_pipe;
    bool m_is_open;

    MessageReceivedCallback m_message_received_handler;

    bool wait_for_connection();
    std::string read_message();

public:
    PipeChannel(std::string&& pipe_name);

    void open();
    void close();
    void reply(std::string&& message) const;

    void on_message_received(MessageReceivedCallback&& message_received_handler);

    ~PipeChannel();
};
