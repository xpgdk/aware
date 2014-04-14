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
#include <aware/android/io_service.hpp>
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
        awareObject(jniEnv->NewGlobalRef(awareObject)) {
    }

    virtual ~factory() {
        jniEnv->DeleteGlobalRef(awareObject);
    }

    boost::shared_ptr<aware::io_service> make_service(boost::asio::io_service& io) {
        return boost::make_shared<aware::android::io_service>(boost::ref(io), jniEnv, awareObject);
    }

    boost::shared_ptr<aware::announce_socket> make_announce(aware::io_service& aio) {
        return boost::make_shared<aware::android::announce_socket>((boost::ref(dynamic_cast<aware::android::io_service&>(aio))));
    }

    boost::shared_ptr<aware::monitor_socket> make_monitor(aware::io_service& aio) {
        return boost::make_shared<aware::android::monitor_socket>(boost::ref(dynamic_cast<aware::android::io_service&>(aio)));
    }

private:
    JNIEnv* jniEnv;
    jobject awareObject;
};

} // namespace android
} // namespace aware

#endif // AWARE_ANDROID_FACTORY_HPP
