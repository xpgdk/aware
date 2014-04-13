#ifndef AWARE_ANDROID_DETAIL_AWARE_JNI_HPP
#define AWARE_ANDROID_DETAIL_AWARE_JNI_HPP

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
#include <set>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <aware/android/detail/monitor.hpp>

namespace aware
{
namespace android
{
namespace detail
{

class client;
class service_subscription;

typedef boost::shared_ptr<aware::android::detail::service_subscription> service_subscription_ptr;
typedef boost::weak_ptr<aware::android::detail::service_subscription> service_subscription_weak_ptr;

class aware_jni
{
    friend class service_subscription;
    typedef std::map<std::string, service_subscription_weak_ptr> service_subscription_map;

public:
    typedef std::pair<boost::system::error_code, aware::contact> response_type;

    aware_jni(JNIEnv* env, jobject java_self);
    ~aware_jni();

    service_subscription_ptr subscribe_service(const std::string& service_type, monitor_ptr monitor);

    void register_client(client* client) {
        std::cerr << __PRETTY_FUNCTION__ << std::endl;;
        clients.insert(client);
    }

    void unregister_client(client* client) {
        std::cerr << __PRETTY_FUNCTION__ << std::endl;;
        clients.erase(client);
    }

    void deliver_response(const response_type& response);

private:
    void listen_for_service(const std::string& service_type);
    void stop_listen(const std::string& service_type);

private:
    JNIEnv* jni_env;
    jobject java_self;
    std::set<client*> clients;
    service_subscription_map service_subscribers;
};

class service_subscription
{
    typedef std::set<monitor_weak_ptr> monitors_type;
public:
    service_subscription(aware_jni& aware, const std::string& service_type);
    ~service_subscription();
    void add_monitor(monitor_weak_ptr monitor);

    void deliver_response(const aware_jni::response_type& response);

private:
    aware_jni& aware;
    const std::string service_type;
    monitors_type monitors;
};

} // namespace detail
} // namespace android
} // namespace aware

#endif // AWARE_ANDROID_FACTORY_HPP
