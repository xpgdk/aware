package dk.xpg.aware;

public interface AwareAdapter {
    public void startDiscover(AwareContact contact);
    public void stopDiscover(AwareContact contact);
    public void startAnnouncement(AwareContact contact);
    public void stopAnnouncement(AwareContact contact);
}
