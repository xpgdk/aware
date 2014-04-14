#include <jni.h>
#include <iostream>

#include <boost/asio/ip/tcp.hpp>

#include <aware/contact.hpp>
#include <aware/android/detail/aware_jni.hpp>

using namespace aware::android::detail;

static aware_jni* get_aware_jni(JNIEnv* env, jobject self)
{
    jfieldID nativeDataId = env->GetFieldID(env->GetObjectClass(self), "nativeData", "J");
    jlong nd = env->GetLongField(self, nativeDataId);
    return reinterpret_cast<aware_jni*>(nd);
}

static boost::asio::ip::tcp::endpoint convert_endpoint(JNIEnv* env, jobject host, int port)
{
    if (host == NULL) {
        return boost::asio::ip::tcp::endpoint();
    }

    jclass address_type = env->GetObjectClass(host);
    jmethodID get_address_id = env->GetMethodID(address_type, "getAddress", "()[B");

    jclass inet6_type = env->FindClass("java/net/Inet6Address");

    boost::asio::ip::address address;

    jbyteArray address_bytes_java = static_cast<jbyteArray>(env->CallObjectMethod(host, get_address_id));
    jbyte* address_bytes = env->GetByteArrayElements(address_bytes_java, NULL);
    jint java_array_size = env->GetArrayLength(address_bytes_java);

    boost::asio::ip::address_v6::bytes_type v6_addr;
    boost::asio::ip::address_v4::bytes_type v4_addr;

    if (inet6_type == address_type && java_array_size == static_cast<int>(v6_addr.size())) {
        for(size_t i=0; i<v6_addr.size(); i++) {
            v6_addr[i] = address_bytes[i];
        }
        address = boost::asio::ip::address_v6(v6_addr);
    } else if (java_array_size == static_cast<int>(v4_addr.size())) {
        for(size_t i=0; i<v4_addr.size(); i++) {
            v4_addr[i] = address_bytes[i];
        }
        address = boost::asio::ip::address_v4(v4_addr);
    }

    env->ReleaseByteArrayElements(address_bytes_java, address_bytes, JNI_ABORT);

    return boost::asio::ip::tcp::endpoint(address, port);
}

static std::string convert_string(JNIEnv* env, jstring string)
{
    if (string == NULL) {
        return std::string();
    }

    const char *c_str = env->GetStringUTFChars(string, 0);
    std::string res(c_str);
    env->ReleaseStringUTFChars(string, c_str);

    return res;
}

static aware::contact convert_contact(JNIEnv* env, jobject java_contact)
{
    jmethodID get_service_name_id = env->GetMethodID(env->GetObjectClass(java_contact), "getServiceName", "()Ljava/lang/String;");
    jmethodID get_service_type_id = env->GetMethodID(env->GetObjectClass(java_contact), "getServiceType", "()Ljava/lang/String;");
    jmethodID get_port_id = env->GetMethodID(env->GetObjectClass(java_contact), "getPort", "()I");
    jmethodID get_host_id = env->GetMethodID(env->GetObjectClass(java_contact), "getHost", "()Ljava/net/InetAddress;");

    jobject service_name = env->CallObjectMethod(java_contact, get_service_name_id);
    jobject service_type = env->CallObjectMethod(java_contact, get_service_type_id);
    jobject host = env->CallObjectMethod(java_contact, get_host_id);
    int port = env->CallIntMethod(java_contact, get_port_id);

    std::string name = convert_string(env, static_cast<jstring>(service_name));
    std::string type = convert_string(env, static_cast<jstring>(service_type));
    boost::asio::ip::tcp::endpoint endpoint = convert_endpoint(env, host, port);

    return aware::contact(name, type, endpoint);
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_Aware_nativeCreate(JNIEnv* env, jobject self)
{
    aware_jni *awarejni = new aware_jni(env, self);

    jfieldID native_data_id = env->GetFieldID(env->GetObjectClass(self), "nativeData", "J");
    env->SetLongField(self, native_data_id, reinterpret_cast<jlong>(awarejni));
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_Aware_nativeDestroy(JNIEnv* env, jobject self)
{
    aware_jni *awarejni = get_aware_jni(env, self);
    delete awarejni;
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_Aware_serviceJoin(JNIEnv* env, jobject self, jobject java_contact)
{
    aware_jni *awarejni = get_aware_jni(env, self);

    aware::contact contact = convert_contact(env, java_contact);
    boost::system::error_code success;
    aware_jni::response_type response = std::make_pair(success, contact);
    awarejni->deliver_response(response);
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_Aware_serviceLeave(JNIEnv* env, jobject self, jobject java_contact)
{
    aware_jni *awarejni = get_aware_jni(env, self);

    aware::contact contact = convert_contact(env, java_contact);
    boost::system::error_code success;
    aware_jni::response_type response = std::make_pair(success, contact);
    awarejni->deliver_response(response);
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_Aware_failure(JNIEnv* env, jobject self, jobject java_contact, jint error_code)
{
    aware_jni *awarejni = get_aware_jni(env, self);

    aware::contact contact = convert_contact(env, java_contact);
    boost::system::error_code error(error_code, boost::system::system_category());
    aware_jni::response_type response = std::make_pair(error, contact);
    awarejni->deliver_response(response);
}
