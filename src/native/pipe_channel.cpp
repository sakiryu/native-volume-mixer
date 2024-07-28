#include "pipe_channel.h"
#include "utils.h"

#include <Windows.h>

#include <array>
#include <format>
#include <stdexcept>

constexpr auto MaxIOSize = 256;

PipeChannel::PipeChannel(std::string&& channel_name) :
    m_pipe{ INVALID_HANDLE_VALUE },
    m_is_open{ false },
    m_message_received_handler{}
{
    const auto pipe_name = std::format("\\\\.\\pipe\\{}", channel_name);
    m_pipe = CreateNamedPipeA(
        pipe_name.c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,
        MaxIOSize,
        MaxIOSize,
        0,
        nullptr);

    if (m_pipe == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Failed to create named pipe");
    }
}

bool PipeChannel::wait_for_connection()
{
    if (!ConnectNamedPipe(m_pipe, nullptr))
    {
        CloseHandle(m_pipe);
        return false;
    }
    return true;
}

std::string PipeChannel::read_message()
{
    std::array<char, MaxIOSize> buffer{};
    auto bytes_read{ 0ul };

    if (!ReadFile(m_pipe, buffer.data(), MaxIOSize - 1, &bytes_read, nullptr))
    {
        return {};
    }

    buffer[bytes_read] = '\0';
    return buffer.data();
}

void PipeChannel::open()
{
    if (!m_message_received_handler)
    {
        throw std::runtime_error("A handler for the 'message received' event must be defined before the channel can be opened.");
    }

    if (!wait_for_connection())
    {
        const auto error_message = std::format("Failed to connect to named pipe. Error: {}", GetLastError());
        throw std::runtime_error(error_message);
    }

    if (m_is_open)
    {
        return;
    }
    
    m_is_open = true;

    while (m_is_open)
    {
        auto message_payload = read_message();
        if (message_payload.empty())
        {
            continue;
        }

        const auto message = utils::split(std::move(message_payload));
        m_message_received_handler(*this, message);
    }
}

void PipeChannel::close()
{
    m_is_open = false;
}

void PipeChannel::reply(std::string&& message) const
{
    DWORD bytes_written{};
    WriteFile(m_pipe, message.c_str(), static_cast<DWORD>(message.size()), &bytes_written, nullptr);
}

void PipeChannel::on_message_received(MessageReceivedCallback&& message_received_handler)
{
    m_message_received_handler = std::move(message_received_handler);
}

PipeChannel::~PipeChannel()
{
    if (m_pipe == INVALID_HANDLE_VALUE)
    {
        return;
    }

    CloseHandle(m_pipe);
    m_pipe = INVALID_HANDLE_VALUE;
}