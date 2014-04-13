#include <aware/android/monitor_socket.hpp>
#include <aware/android/detail/client.hpp>
#include <aware/android/detail/monitor.hpp>
#include <aware/android/detail/aware_jni.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace aware
{
namespace android
{
namespace detail
{

} // namespace detail

void monitor_socket::async_listen(const aware::contact& contact,
                                  async_listen_handler handler) {
    const std::string& key = contact.get_type();
    monitor_map::iterator where = monitors.lower_bound(key);
    if ((where == monitors.end()) || (monitors.key_comp()(key, where->first)))
    {
        where = monitors.insert(where,
                                monitor_map::value_type(key, aware::android::detail::monitor::create(io, contact)));
    }
    assert(where != monitors.end());

    where->second->add_handler(handler);
}

} // namespace android
} // namepsace aware
