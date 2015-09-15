///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2014 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <aware/android/service.hpp>

namespace aware
{
namespace android
{

boost::asio::io_service::id service::id;

service::service(boost::asio::io_service& io, JNIEnv* env, jobject awareObject)
    : boost::asio::io_service::service(io),
      client(env, awareObject)
{}

service::service(boost::asio::io_service& io)
    : boost::asio::io_service::service(io),
      client(NULL, NULL)
{
    assert(false);
}

} // namespace android
} // namespace aware
