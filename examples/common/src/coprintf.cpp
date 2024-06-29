#include <coprintf.hpp>

#ifdef __gnu_linux__

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <algorithm>
#include <memory>
#include <string>
#include <chrono>




class CoPrintfMessage final : public CoPrintfBaseMessage
{
public:
    enum class PacketType
    {
        MiddlePacket = 0,
        FirstPacket  = 1,
        LastPacket   = 2
    };

    struct MessagePacket
    {
        uint32_t size;
        uint16_t id;
        PacketType packet_type;
        uint8_t pd[1];
        uint64_t uid;
        uint64_t nano;
    };

    constexpr static size_t max_packet_size = 1400 - sizeof(MessagePacket);

public:
    CoPrintfMessage(int fd, sockaddr_in addr)
        : _fd(fd), _next_id(0), _uid(make_uid()), _addr(addr), _total_size(0)
    {}
    ~CoPrintfMessage()
    {
        write_packet("", 0, 0, PacketType::LastPacket);
        flush();
    }

    void write(const void* data, size_t size) override;
    void flush() override;

private:
    static uint64_t make_uid()
    {
        srand(std::chrono::steady_clock::now().time_since_epoch().count());
        return (uint64_t)(rand() + (rand() << 16) + ((uint64_t)rand() << 32) + ((uint64_t)rand() << 48));
    }

    std::unique_ptr<uint8_t[]> new_packet(const void* data, size_t size, uint16_t id, PacketType packet_type, uint64_t nano = 0)
    {
        using namespace std::chrono;
        std::unique_ptr<uint8_t[]> buffer{new uint8_t[size + sizeof(MessagePacket)]};
        MessagePacket* packet = (MessagePacket*)buffer.get();
        packet->size = size + sizeof(MessagePacket);
        packet->id = id;
        packet->packet_type = packet_type;
        packet->uid = _uid;
        packet->nano = (nano ? nano : duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count());
        memcpy(packet + 1, data, size);
        return buffer;
    }
    void write_packet(const void* data, size_t size, uint64_t nano = 0, PacketType packet_type = PacketType::MiddlePacket)
    {
        if (_total_size + size > max_packet_size)
            flush();
        auto pt = (packet_type != PacketType::MiddlePacket ? packet_type : (_next_id == 0 ? PacketType::FirstPacket : PacketType::MiddlePacket));
        _buffer.emplace_back(new_packet(data, size, _next_id++, pt, nano));
        _total_size += size + sizeof(MessagePacket);
    }

private:
    int _fd;
    int _next_id;
    uint64_t _uid;
    sockaddr_in _addr;
    std::vector<std::unique_ptr<uint8_t[]>> _buffer;
    size_t _total_size;

};


void coprintf(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    covprintf(fmt, va);
    va_end(va);
}

void covprintf(const char* fmt, va_list args)
{
    char buffer[32 * 1024];
    auto size = vsnprintf(buffer, sizeof(buffer), fmt, args);
    auto msg = coprintf_make_message();
    msg->write(buffer, size);
    msg->flush();
}


static
int get_sink_fd()
{
    static int fd = -1;
    if (fd < 0)
    {
        fd = socket(AF_INET, SOCK_DGRAM, 0);
    }
    return fd;
}

class CoPrintfTroubleBuffer : public CoPrintfBaseMessage
{
public:
    CoPrintfTroubleBuffer(CoPrintfBaseMessage* bind = nullptr) //! SRP solid
        : _bind(bind) {}

    void write(const void* data, size_t size) override
    {
        if (_bind)
            _bind->write(data, size);
        else
            _data.push_back(std::string((const char*)data, (const char*)data + size));
    }

    void resend(CoPrintfBaseMessage* msg)
    {
        for (auto& pkg : _data)
            msg->write(pkg.data(), pkg.size());
    }

private:
    CoPrintfBaseMessage* _bind;
    std::vector<std::string> _data;

};

std::unique_ptr<CoPrintfBaseMessage> coprintf_make_message()
{
    static sockaddr_in addr = { AF_INET, htons(16444), inet_addr("127.0.0.1") };
    static std::vector<std::unique_ptr<CoPrintfTroubleBuffer>> trouble_buffer;
    int fd = get_sink_fd();
    if (fd < 0)
    {
        auto buf = std::make_unique<CoPrintfTroubleBuffer>();
        auto rem = std::make_unique<CoPrintfTroubleBuffer>(buf.get());
        trouble_buffer.emplace_back(std::move(buf));
        return rem;
    }
    else {
        if (trouble_buffer.size())
        {
            for (auto& buf : trouble_buffer)
            {
                auto msg = std::make_unique<CoPrintfMessage>(fd, addr);
                buf->resend(msg.get());
            }
        }
        return std::make_unique<CoPrintfMessage>(fd, addr);
    }
}

void CoPrintfMessage::write(const void* data, size_t size)
{
    using namespace std::chrono;
    auto nano = duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
    while (size > 0)
    {
        auto next_size = std::min<size_t>(max_packet_size, size);
        write_packet(data, next_size, nano);
        data = (char*)data + next_size;
        size -= next_size;
    }
}

void CoPrintfMessage::flush()
{
    char buffer[4096];
    char* it = buffer;
    for (auto& buf : _buffer)
    {
        auto sz = reinterpret_cast<const MessagePacket*>(buf.get())->size;
        memcpy(it, buf.get(), sz);
        it += sz;
    }
    sendto(_fd, buffer, _total_size, 0, (const sockaddr*)&_addr, sizeof(_addr));
    _total_size = 0;
    _buffer.clear();
}

#else

void coprintf(const char*, ...) {}
void covprintf(const char*, ...) {}

#endif
