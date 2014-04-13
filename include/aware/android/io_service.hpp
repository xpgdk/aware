#ifndef AWARE_ANDROID_IO_SERVICE_HPP
#define AWARE_ANDROID_IO_SERVICE_HPP

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

#include <boost/asio/io_service.hpp>

#include <aware/io_service.hpp>
#include <jni.h>

#include <aware/android/detail/client.hpp>

namespace aware
{
namespace android
{

class announce_socket;
class monitor_socket;

class io_service : public aware::io_service
{
public:
    io_service(boost::asio::io_service& io, JNIEnv* env, jobject awareObject)
        : aware::io_service(io),
          client(env, awareObject)
    {}

    const detail::client& get_client() const { return client; }

private:
    detail::client client;
};

} // namespace avahi
} // namespace aware

#endif // AWARE_ANDROID_IO_SERVICE_HPP
