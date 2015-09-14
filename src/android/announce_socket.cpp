#include <aware/android/announce_socket.hpp>

namespace aware
{
namespace android
{

announce_socket::announce_socket(boost::asio::io_service& ios)
    : boost::asio::basic_io_object<android::service>(ios),
      ios(ios)
{
}

void announce_socket::async_announce(const aware::contact& contact,
                                     async_announce_handler handler)
{
    announcement = boost::asio::use_service<android::service>(ios).get_client().add_announcement(contact);
}

} // namespace android
} // namespace aware
