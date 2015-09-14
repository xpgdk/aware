#include <iostream>

#include <jni.h>

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include <aware/contact.hpp>
#include <aware/android/factory.hpp>

static aware::android::factory *factory;
static boost::shared_ptr<boost::asio::io_service> ios;
static boost::shared_ptr<aware::monitor_socket> monitor_socket;
static boost::thread io_thread;
static boost::shared_ptr<boost::asio::io_service::work> work;

static boost::shared_ptr<aware::announce_socket> announce_socket;

static volatile bool running;

void io_function()
{
    ios->run();
}

void announce_handler(const boost::system::error_code& error)
{
}

void process_listen(const boost::system::error_code& error, const aware::contact& contact)
{
    if (!error) {
        if (contact.get_endpoint().address().is_unspecified()) {
            std::cerr << "Lost service: " << contact.get_type() 
                      << ", " << contact.get_name() << std::endl;
        } else {
            std::cerr << "Got service: " << contact.get_type() 
                      << ", " << contact.get_name()
                      << ", endpoint: " << contact.get_endpoint() << std::endl;
        }
        if (monitor_socket) {
            monitor_socket->async_listen(contact, boost::bind(process_listen, boost::asio::placeholders::error, _2));
        }
    }
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_test_AwareTest_initTest(JNIEnv* env, jobject self, jobject aware_object)
{
    ios = boost::make_shared<boost::asio::io_service>();
    work = boost::make_shared<boost::asio::io_service::work>(boost::ref(*ios));

    factory = new aware::android::factory(env, aware_object);
    monitor_socket = factory->make_monitor(*ios);

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
    //aio.reset();
    delete factory;
    ios->stop();
    io_thread.join();
    ios.reset();
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_test_AwareTest_nativeStartAnnouncement(JNIEnv* env, jobject self)
{
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("10.0.0.1"), 321);
    aware::contact contact("This is my name", "announce", endpoint);

    announce_socket = factory->make_announce(*ios);
    announce_socket->async_announce(contact, boost::bind(announce_handler, boost::asio::placeholders::error));
}

extern "C" JNIEXPORT
void JNICALL Java_dk_xpg_aware_test_AwareTest_nativeStopAnnouncement(JNIEnv* env, jobject self)
{
    announce_socket.reset();
}
