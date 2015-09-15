#ifndef AWARE_ANDROID_SERVICE_HPP
#define AWARE_ANDROID_SERVICE_HPP

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

#include <map>

#include <boost/asio/io_service.hpp>

#include <jni.h>

#include <aware/android/detail/client.hpp>

namespace aware
{
namespace android
{

class announce_socket;
class monitor_socket;

class service : public boost::asio::io_service::service
{
public:
    static boost::asio::io_service::id id;

    struct implementation_type
    {
    };

    service(boost::asio::io_service& io, JNIEnv* env, jobject awareObject);
    service(boost::asio::io_service& io);

    void construct(implementation_type&) {}
    void destroy(implementation_type&) {}

    const detail::client& get_client() const { return client; }

private:
    virtual void shutdown_service() {}

private:
    detail::client client;
};

} // namespace avahi
} // namespace aware

#endif // AWARE_ANDROID_SERVICE_HPP
