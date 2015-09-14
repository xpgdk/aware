#ifndef AWARE_ANDROID_MONITOR_SOCKET_HPP
#define AWARE_ANDROID_MONITOR_SOCKET_HPP

///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <aware/android/service.hpp>
#include <aware/contact.hpp>
#include <aware/monitor_socket.hpp>

namespace aware
{
namespace android
{

namespace detail { class monitor; }

class monitor_socket : public aware::monitor_socket
{
    typedef boost::shared_ptr<aware::android::detail::monitor> monitor_ptr;
    typedef std::map<std::string, monitor_ptr> monitor_map;

public:
    typedef boost::function<void (const boost::system::error_code&,
                                  const aware::contact&)> async_listen_handler;

    monitor_socket(boost::asio::io_service& io) : io(io) {}

    void async_listen(const aware::contact& contact,
                      async_listen_handler handler);

private:
    boost::asio::io_service& io;
    monitor_map monitors; // FIXME: thread-safety
};

} // namespace android
} // namespace aware

#endif // AWARE_ANDROID_MONITOR_SOCKET_HPP
