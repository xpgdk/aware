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

service_subscription_ptr aware_jni::subscribe_service(const aware::contact& contact, monitor_ptr monitor)
{
    service_subscription_ptr tmp;
    
    const std::string service_type = contact.get_type();

    service_subscription_map::iterator where = service_subscribers.lower_bound(service_type);
    if ((where == service_subscribers.end()) || (service_subscribers.key_comp()(service_type, where->first)))
    {
        tmp = boost::make_shared<aware::android::detail::service_subscription>(boost::ref(*this), contact);
        boost::weak_ptr<aware::android::detail::service_subscription> stored_value(tmp);
        where = service_subscribers.insert(where,
                                           service_subscription_map::value_type(service_type,stored_value));
    }
    assert(where != service_subscribers.end());

    service_subscription_ptr res(where->second);

    res->add_monitor(monitor);

    return res;
}

service_announcement_ptr aware_jni::announce_service(const aware::contact& contact)
{
    service_announcement_ptr res = boost::make_shared<aware::android::detail::service_announcement>(boost::ref(*this), contact);
    return res;
}

void aware_jni::listen_for_service(const aware::contact& contact)
{
    jmethodID methodId = jni_env->GetMethodID(jni_env->GetObjectClass(java_self), "startDiscover", "(Ldk/xpg/aware/AwareContact;)V");

    jni_env->CallVoidMethod(java_self, methodId, create_java_contact(contact));
}

void aware_jni::stop_listen(const aware::contact& contact)
{
    jmethodID methodId = jni_env->GetMethodID(jni_env->GetObjectClass(java_self), "stopDiscover", "(Ldk/xpg/aware/AwareContact;)V");

    jni_env->CallVoidMethod(java_self, methodId, create_java_contact(contact));
}

void aware_jni::start_announcement(const aware::contact& contact)
{
    jobject java_contact = create_java_contact(contact);
    jmethodID method_id = jni_env->GetMethodID(jni_env->GetObjectClass(java_self), "startAnnouncement", "(Ldk/xpg/aware/AwareContact;)V");
    jni_env->CallVoidMethod(java_self, method_id, java_contact);
}

void aware_jni::stop_announcement(const aware::contact& contact)
{
    jobject java_contact = create_java_contact(contact);
    jmethodID method_id = jni_env->GetMethodID(jni_env->GetObjectClass(java_self), "stopAnnouncement", "(Ldk/xpg/aware/AwareContact;)V");
    jni_env->CallVoidMethod(java_self, method_id, java_contact);
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

jobject aware_jni::create_java_contact(const aware::contact& contact)
{
    jclass java_contact_class = jni_env->FindClass("dk/xpg/aware/AwareContact");
    if (contact.get_endpoint().address().is_unspecified()) {
        // Use 2 argument constructor
        jmethodID constructorId = jni_env->GetMethodID(java_contact_class, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
        return jni_env->NewObject(java_contact_class, 
                                  constructorId, 
                                  jni_env->NewStringUTF(contact.get_type().c_str()),
                                  jni_env->NewStringUTF(contact.get_name().c_str()));

    } else {
        // Use 4 argument constructor
        jmethodID constructorId = jni_env->GetMethodID(java_contact_class, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/net/InetAddress;I)V");

        jint port = contact.get_endpoint().port();
        jobject inet_address;

        boost::asio::ip::address address = contact.get_endpoint().address();

        jclass inet_address_class = jni_env->FindClass("java/net/InetAddress");
        jmethodID get_by_address_id = jni_env->GetStaticMethodID(inet_address_class, "getByAddress", "([B)Ljava/net/InetAddress;");

        if( address.is_v6() ) {
            boost::asio::ip::address_v6 addr = address.to_v6();
            boost::asio::ip::address_v6::bytes_type b = addr.to_bytes();
            jbyteArray data_array = jni_env->NewByteArray(b.size());
            jbyte *data = jni_env->GetByteArrayElements(data_array, NULL);
            for(size_t i=0; i<b.size(); i++) {
                data[i] = b[i];
            }
            jni_env->ReleaseByteArrayElements(data_array, data, 0);
            inet_address = jni_env->CallStaticObjectMethod(inet_address_class, get_by_address_id, data_array);
        } else {
            // Else assume v4
            boost::asio::ip::address_v4 addr = address.to_v4();
            boost::asio::ip::address_v4::bytes_type b = addr.to_bytes();
            jbyteArray data_array = jni_env->NewByteArray(b.size());
            jbyte *data = jni_env->GetByteArrayElements(data_array, NULL);
            for(size_t i=0; i<b.size(); i++) {
                data[i] = b[i];
            }
            jni_env->ReleaseByteArrayElements(data_array, data, 0);
            inet_address = jni_env->CallStaticObjectMethod(inet_address_class, get_by_address_id, data_array);
        }

        return jni_env->NewObject(java_contact_class, 
                                  constructorId, 
                                  jni_env->NewStringUTF(contact.get_type().c_str()),
                                  jni_env->NewStringUTF(contact.get_name().c_str()),
                                  inet_address, port);
    }
}

service_subscription::service_subscription(aware_jni& aware, const aware::contact& contact)
    : aware(aware),
      contact(contact)
{
    aware.listen_for_service(contact);
}

service_subscription::~service_subscription()
{
    aware.stop_listen(contact);
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
