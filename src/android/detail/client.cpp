#include <aware/android/detail/aware_jni.hpp>
#include <aware/android/detail/client.hpp>
#include <aware/android/io_service.hpp>
#include <iostream>

namespace aware
{
namespace android
{
namespace detail
{

monitor_ptr monitor::create(aware::android::io_service& io, const aware::contact& contact)
{
    monitor_ptr instance = boost::make_shared<aware::android::detail::monitor>(boost::ref(io), contact);

    const std::string& type = contact.get_type();
    instance->subscription = io.get_client().add_monitor(instance, type);

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

    // TODO: Rework code so that we call dispatch from io_service thread
    io.post(boost::bind(handler,response.first, response.second));

    responses.pop();
    handlers.pop();
}

client::client(JNIEnv* env, jobject awareObject) 
    : jniEnv(env),
      awareObject(jniEnv->NewGlobalRef(awareObject))
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    aware_jni *awarejni = get_aware_jni();
    std::cerr << "awarejni: " << awarejni << std::endl;
    awarejni->register_client(this);
}

client::~client()
{
    get_aware_jni()->unregister_client(this);
    jniEnv->DeleteGlobalRef(awareObject);
}

aware_jni* client::get_aware_jni() const
{
    jmethodID getNativeDataId = jniEnv->GetMethodID(jniEnv->GetObjectClass(awareObject), "getNativeData", "()J");
    jlong ptr = jniEnv->CallLongMethod(awareObject, getNativeDataId);

    return reinterpret_cast<aware_jni*>(ptr);
}

service_subscription_ptr client::add_monitor(monitor_ptr monitor, const std::string& serviceType) const
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    return get_aware_jni()->subscribe_service(serviceType, monitor);
}

void client::serviceDiscovered(std::string& serviceName)
{
    std::cerr << __PRETTY_FUNCTION__ << ": " << serviceName << std::endl;
}

} // namespace detail
} // namespace android
} // namepsace aware
