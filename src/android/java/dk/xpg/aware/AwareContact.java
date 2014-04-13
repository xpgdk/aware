package dk.xpg.aware;

import java.net.InetAddress;

public class AwareContact
{
    private InetAddress host;
    private int port;
    private String serviceName;
    private String serviceType;

    public AwareContact(String serviceType, String serviceName)
    {
        this.serviceType = serviceType;
        this.serviceName = serviceName;
    }
    
    public AwareContact(String serviceType, String serviceName, InetAddress host, int port) {
        this.host = host;
        this.port = port;
        this.serviceName = serviceName;
        this.serviceType = serviceType;
    }

    public InetAddress getHost() {
        return host;
    }

    public void setHost(InetAddress host) {
        this.host = host;
    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public String getServiceName() {
        return serviceName;
    }

    public void setServiceName(String serviceName) {
        this.serviceName = serviceName;
    }

    public String getServiceType() {
        return serviceType;
    }

    public void setServiceType(String serviceType) {
        this.serviceType = serviceType;
    }
}
