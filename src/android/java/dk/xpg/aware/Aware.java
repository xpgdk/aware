package dk.xpg.aware;

public class Aware {
    private final AwareAdapter adapter;

    public Aware(AwareAdapter adapter) {
        this.adapter = adapter;
    }

    public void create() {
        if (!initialized) {
            nativeCreate();
            initialized = true;
        }
    }

    public void destroy() {
        if (initialized) {
            nativeDestroy();
            initialized = false;
        }
    }

    public long getNativeData() {
        return nativeData;
    }

    private void startDiscover(AwareContact contact) {
        adapter.startDiscover(contact);
    }

    private void stopDiscover(AwareContact contact) {
        adapter.stopDiscover(contact);
    }

    private void startAnnouncement(AwareContact contact) {
        adapter.startAnnouncement(contact);
    }

    private void stopAnnouncement(AwareContact contact) {
        adapter.stopAnnouncement(contact);
    }

    public native void serviceJoin(AwareContact contact);
    public native void serviceLeave(AwareContact contact);
    public native void failure(AwareContact contact, int errorCode);

    private native void nativeCreate();
    private native void nativeDestroy();

    private long nativeData;
    private boolean initialized = false;

    static {
        System.loadLibrary("AwareAndroidJNI");
    }
}
