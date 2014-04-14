#include <aware/android/detail/aware_jni.hpp>

namespace aware
{
namespace android
{
namespace detail
{

aware_jni::aware_jni(JNIEnv* env, jobject java_self)
    : jni_env(env),
      java_self(jni_env->NewGlobalRef(java_self))
{
}

aware_jni::~aware_jni()
{
    jni_env->DeleteGlobalRef(java_self);
}

service_subscription_ptr aware_jni::subscribe_service(const std::string& service_type, monitor_ptr monitor)
{
    service_subscription_ptr tmp;

    service_subscription_map::iterator where = service_subscribers.lower_bound(service_type);
    if ((where == service_subscribers.end()) || (service_subscribers.key_comp()(service_type, where->first)))
    {
        tmp = boost::make_shared<aware::android::detail::service_subscription>(boost::ref(*this), service_type);
        boost::weak_ptr<aware::android::detail::service_subscription> stored_value(tmp);
        where = service_subscribers.insert(where,
                                           service_subscription_map::value_type(service_type,stored_value));
    }
    assert(where != service_subscribers.end());

    service_subscription_ptr res(where->second);

    res->add_monitor(monitor);

    return res;
}

void aware_jni::listen_for_service(const std::string& service_type)
{
    jmethodID methodId = jni_env->GetMethodID(jni_env->GetObjectClass(java_self), "discover", "(Ljava/lang/String;)V");

    jni_env->CallVoidMethod(java_self, methodId, jni_env->NewStringUTF(service_type.c_str()));
}

void aware_jni::stop_listen(const std::string& service_type)
{
    jmethodID methodId = jni_env->GetMethodID(jni_env->GetObjectClass(java_self), "stopDiscover", "(Ljava/lang/String;)V");

    jni_env->CallVoidMethod(java_self, methodId, jni_env->NewStringUTF(service_type.c_str()));
}

void aware_jni::deliver_response(const response_type& response)
{
    service_subscription_map::iterator it = service_subscribers.find(response.second.get_type());
    if (it != service_subscribers.end() ) {
        if (!it->second.expired()) {
            it->second.lock()->deliver_response(response);
        } else {
            service_subscribers.erase(it);
        }
    }
}

service_subscription::service_subscription(aware_jni& aware, const std::string& service_type)
    : aware(aware),
      service_type(service_type)
{
    aware.listen_for_service(service_type);
}

service_subscription::~service_subscription()
{
    aware.stop_listen(service_type);
}

void service_subscription::add_monitor(monitor_weak_ptr monitor)
{
    monitors.insert(monitor);
}

void service_subscription::deliver_response(const aware_jni::response_type& response)
{
    for(monitors_type::iterator it = monitors.begin(); it != monitors.end(); it++) {
        if (!it->expired()) {
            it->lock()->deliver_response(response);
        }
    }
}

} // namespace detail
} // namespace android
} // namespace aware
