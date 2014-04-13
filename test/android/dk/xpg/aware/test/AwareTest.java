package dk.xpg.aware.test;

//import dk.xpg.aware.Aware;
//import dk.xpg.aware.AwareAdapter;

public class AwareTest/* implements AwareAdapter*/ {

    public static void main(String[] args) {
        AwareTest test = new AwareTest();
        /*Aware aware = new Aware(test);
        aware.create();

        test.initTest();

        aware.destroy();*/
    }

    public void discover(String serviceType) {
        System.err.println("Discovery " + serviceType);
    }

    native void initTest();
    native void performTest();

    static {
        System.loadLibrary("android_test_jni");
    }
}
