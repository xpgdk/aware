package dk.xpg.aware.test;

import dk.xpg.aware.Aware;
import dk.xpg.aware.AwareAdapter;
import dk.xpg.aware.AwareContact;
import java.net.InetAddress;

public class AwareTest implements AwareAdapter {

    public static void main(String[] args) throws Exception {
        AwareTest test = new AwareTest();
        Aware aware = new Aware(test);
        aware.create();

        test.initTest(aware);
        System.err.println("Test initalized");

        test.performTest();

        AwareContact contact = new AwareContact("announce", "Testing");
        contact.setHost(InetAddress.getLocalHost());
        contact.setPort(121);

        aware.serviceJoin(contact);
        Thread.sleep(1000);
        aware.serviceLeave(new AwareContact("announce", "Testing"));

        Thread.sleep(1000);
        test.destroyMonitorSocket();
        Thread.sleep(1000);

        aware.serviceJoin(contact);
        aware.serviceJoin(contact);

        test.nativeStartAnnouncement();
        Thread.sleep(1000);
        test.nativeStopAnnouncement();

        test.testDone();
        aware.destroy();
        System.err.println("Aware destroyed");
    }

    public void startDiscover(AwareContact contact) {
        System.err.println("startDiscover: " + contact);
    }

    public void stopDiscover(AwareContact contact) {
        System.err.println("stopDiscover: " + contact);
    }

    public void startAnnouncement(AwareContact contact) {
        System.err.println("startAnnouncement: " + contact);
    }

    public void stopAnnouncement(AwareContact contact) {
        System.err.println("stopAnnouncement: " + contact);
    }

    native void initTest(Aware aware);
    native void destroyMonitorSocket();
    native void performTest();
    native void testDone();
    native void nativeStartAnnouncement();
    native void nativeStopAnnouncement();

    static {
        System.loadLibrary("android_test_jni");
    }
}
