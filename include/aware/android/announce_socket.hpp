#ifndef AWARE_ANDROID_ANNOUNCE_SOCKET_HPP
#define AWARE_ANDROID_ANNOUNCE_SOCKET_HPP

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

#include <boost/function.hpp>
#include <aware/android/io_service.hpp>
#include <aware/contact.hpp>

#include <aware/announce_socket.hpp>

namespace aware
{
namespace android
{

class io_service;

class announce_socket : public aware::announce_socket
{
public:
    typedef boost::function<void (const boost::system::error_code&)> async_announce_handler;

    announce_socket(aware::android::io_service&) {
    }

    void async_announce(const aware::contact& contact,
                        async_announce_handler) {
    }

private:
};

} // namespace avahi
} // namespace aware

#endif // AWARE_ANDROID_ANNOUNCE_SOCKET_HPP
