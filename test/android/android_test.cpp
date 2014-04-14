#include <iostream>

#include <jni.h>

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include <aware/contact.hpp>
#include <aware/android/factory.hpp>

static aware::android::factory *factory;
static boost::asio::io_service ios;
static boost::shared_ptr<aware::io_service> aio;
static boost::shared_ptr<aware::monitor_socket> monitor_socket;
static boost::thread io_thread;
static boost::shared_ptr<boost::asio::io_service::work> work;

static volatile bool running;

void io_function()
{
    ios.run();
}

void process_listen(const boost::system::error_code& error, const aware::contact& contact)
{
    if (!error) {
        if (contact.get_endpoint().address().is_unspecified()) {
            std::cerr << "Lost service: " << contact.get_type() << std::endl;
        } else {
            std::cerr << "Got service: " << contact.get_type() << std::endl;
        }
        if (monitor_socket) {
            monitor_socket->async_listen(contact, boost::bind(process_listen, boost::asio::placeholders::error, _2));
        }
    }
}

void test_method()
{
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_test_AwareTest_initTest(JNIEnv* env, jobject self, jobject aware_object)
{

    work = boost::make_shared<boost::asio::io_service::work>(boost::ref(ios));

    factory = new aware::android::factory(env, aware_object);
    aio = factory->make_service(ios);
    monitor_socket = factory->make_monitor(*aio);

    running = true;
    io_thread = boost::thread(io_function);
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_test_AwareTest_performTest(JNIEnv* env, jobject self)
{
    aware::contact contact("", "announce");
    monitor_socket->async_listen(contact, boost::bind(process_listen, boost::asio::placeholders::error, _2));
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_test_AwareTest_destroyMonitorSocket(JNIEnv* env, jobject self)
{
    monitor_socket.reset();
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_test_AwareTest_testDone(JNIEnv* env, jobject self)
{

    work.reset();

    monitor_socket.reset();
    aio.reset();
    delete factory;
    ios.stop();
}
