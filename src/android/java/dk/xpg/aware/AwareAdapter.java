package dk.xpg.aware;

public interface AwareAdapter {
    public void discover(String serviceType);
    public void stopDiscover(String serviceType);
    public void startAnnouncement(AwareContact contact);
    public void stopAnnouncement(AwareContact contact);
}
