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
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    ios.run();
}

void process_listen(const boost::system::error_code& error, const aware::contact& contact)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    if (!error) {
        std::cerr << "Got service: " << contact.get_type() << std::endl;
    }
}

void test_method()
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_test_AwareTest_initTest(JNIEnv* env, jobject self, jobject aware_object)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;

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
    std::cerr << __PRETTY_FUNCTION__ << std::endl;

    aware::contact contact("", "announce");
    monitor_socket->async_listen(contact, boost::bind(process_listen, boost::asio::placeholders::error, _2));
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_test_AwareTest_testDone(JNIEnv* env, jobject self)
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;

    work.reset();

    monitor_socket.reset();
    aio.reset();
    delete factory;
    ios.stop();
}
