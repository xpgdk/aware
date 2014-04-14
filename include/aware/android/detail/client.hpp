#ifndef AWARE_ANDROID_DETAIL_CLIENT_HPP
#define AWARE_ANDROID_DETAIL_CLIENT_HPP

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


#include <jni.h>
#include <aware/monitor_socket.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <queue>
#include <aware/android/detail/aware_jni.hpp>
#include <aware/android/detail/monitor.hpp>

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

class aware_jni;
class client;

// Client handles all sockets for a single aware::android::io_service.
class client 
{
public:
    client(JNIEnv* env, jobject awareObject);
    ~client();

    service_subscription_ptr add_monitor(monitor_ptr monitor, const std::string& serviceType) const;

private:
    aware_jni* get_aware_jni() const;

private:
    JNIEnv* jniEnv;
    jobject awareObject;
};

} // namespace detail
} // namespace android
} // namespace aware

#endif // AWARE_ANDROID_FACTORY_HPP
