package dk.xpg.aware;

public interface AwareAdapter {
    public void discover(String serviceType);
    public void stopDiscover(String serviceType);
}
