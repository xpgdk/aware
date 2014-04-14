#include <aware/android/announce_socket.hpp>

namespace aware
{
namespace android
{

announce_socket::announce_socket(aware::android::io_service& ios)
    : ios(ios)
{
}

void announce_socket::async_announce(const aware::contact& contact,
                                     async_announce_handler handler)
{
    announcement = ios.get_client().add_announcement(contact);
}

} // namespace android
} // namespace aware
