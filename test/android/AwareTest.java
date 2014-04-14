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

        aware.serviceLeave(new AwareContact("announce", "Testing"));

        Thread.sleep(1000);
        test.destroyMonitorSocket();

        aware.serviceJoin(contact);
        aware.serviceJoin(contact);

        test.testDone();
        aware.destroy();
        System.err.println("Aware destroyed");
    }

    public void discover(String serviceType) {
        System.err.println("discovery " + serviceType);
    }

    public void stopDiscover(String serviceType) {
        System.err.println("stopDiscovery " + serviceType);
    }

    native void initTest(Aware aware);
    native void destroyMonitorSocket();
    native void performTest();
    native void testDone();

    static {
        System.loadLibrary("android_test_jni");
    }
}
