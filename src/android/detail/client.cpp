#include <aware/android/detail/aware_jni.hpp>
#include <aware/android/detail/client.hpp>
#include <aware/android/service.hpp>
#include <iostream>

namespace aware
{
namespace android
{
namespace detail
{

monitor_ptr monitor::create(boost::asio::io_service& io, const aware::contact& contact)
{
    monitor_ptr instance = boost::make_shared<aware::android::detail::monitor>(boost::ref(io), contact);

    instance->subscription = boost::asio::use_service<android::service>(io).get_client().add_monitor(instance, contact);

    return instance;
}

monitor::~monitor()
{
}

void monitor::dispatch()
{
    if (responses.empty())
        return;
    if (handlers.empty())
        return;

    const response_type& response = responses.front();
    handler_type& handler = handlers.front();

    // TODO: Rework code so that we call dispatch from service thread
    io.post(boost::bind(handler,response.first, response.second));

    responses.pop();
    handlers.pop();
}

client::client(JNIEnv* env, jobject awareObject) 
    : jniEnv(env),
      awareObject(jniEnv->NewGlobalRef(awareObject))
{
}

client::~client()
{
    jniEnv->DeleteGlobalRef(awareObject);
}

aware_jni* client::get_aware_jni() const
{
    jmethodID getNativeDataId = jniEnv->GetMethodID(jniEnv->GetObjectClass(awareObject), "getNativeData", "()J");
    jlong ptr = jniEnv->CallLongMethod(awareObject, getNativeDataId);

    return reinterpret_cast<aware_jni*>(ptr);
}

service_subscription_ptr client::add_monitor(monitor_ptr monitor, const aware::contact& contact) const
{
    return get_aware_jni()->subscribe_service(contact, monitor);
}

service_announcement_ptr client::add_announcement(const aware::contact& contact) const
{
    return get_aware_jni()->announce_service(contact);
}

} // namespace detail
} // namespace android
} // namepsace aware
