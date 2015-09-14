#ifndef AWARE_ANDROID_FACTORY_HPP
#define AWARE_ANDROID_FACTORY_HPP

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

#include <aware/factory.hpp>
#include <aware/android/service.hpp>
#include <aware/android/announce_socket.hpp>
#include <aware/android/monitor_socket.hpp>

#include <boost/make_shared.hpp>

#include <jni.h>

namespace aware
{
namespace android
{

class factory : public aware::factory
{
public:
    factory(JNIEnv* env, jobject awareObject) 
      : jniEnv(env),
        awareObject(jniEnv->NewGlobalRef(awareObject))
    {
    }

    virtual ~factory() {
        jniEnv->DeleteGlobalRef(awareObject);
    }

    boost::shared_ptr<aware::announce_socket> make_announce(boost::asio::io_service& io)
    {
        ensure_service(io);
        return boost::make_shared<aware::android::announce_socket>((boost::ref(io)));
    }

    boost::shared_ptr<aware::monitor_socket> make_monitor(boost::asio::io_service& io)
    {
        ensure_service(io);
        return boost::make_shared<aware::android::monitor_socket>(boost::ref(io));
    }

private:
    void ensure_service(boost::asio::io_service& io)
    {
        if (!boost::asio::has_service<android::service>(io))
        {
            android::service *service = new android::service(io, jniEnv, awareObject);
            boost::asio::add_service<android::service>(io, service);
        }
    }

private:
    JNIEnv* jniEnv;
    jobject awareObject;
};

} // namespace android
} // namespace aware

#endif // AWARE_ANDROID_FACTORY_HPP
