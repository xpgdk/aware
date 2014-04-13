#ifndef AWARE_ANDROID_DETAIL_MONITOR_HPP
#define AWARE_ANDROID_DETAIL_MONITOR_HPP

///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2014 Paul Fleischer <paul@xpg.dk>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////


#include <queue>

#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

#include <aware/monitor_socket.hpp>
#include <aware/contact.hpp>
#include <aware/android/detail/aware_jni.hpp>

namespace aware
{
namespace android
{
    class io_service;
} // namespace android
} // namespace aware

namespace aware
{
namespace android
{
namespace detail
{

class monitor;
class service_subscription;

typedef boost::shared_ptr<monitor> monitor_ptr;
typedef boost::weak_ptr<monitor> monitor_weak_ptr;

class monitor
{
    typedef std::pair<boost::system::error_code, aware::contact> response_type;
    typedef aware::monitor_socket::async_listen_handler handler_type;

public:
    static monitor_ptr create(aware::android::io_service& io, const aware::contact& contact);
    ~monitor();

    void add_handler(handler_type handler)
    {
        handlers.push(handler);
        dispatch();
    }

    void deliver_response(response_type response)
    {
        responses.push(response);
        dispatch();
    }

    void dispatch();

public:
    monitor(aware::android::io_service& io,
            const aware::contact& contact)
        : io(io), contact(contact) {}

private:
    aware::android::io_service& io;
    aware::contact contact;
    std::queue<response_type> responses;
    std::queue<handler_type> handlers;
    boost::shared_ptr<service_subscription> subscription;
};

} // namespace detail
} // namespace android
} // namespace aware

#endif // AWARE_ANDROID_DETAIL_MONITOR_HPP
