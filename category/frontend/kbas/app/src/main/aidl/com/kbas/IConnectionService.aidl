// IConnectionService.aidl
package com.kbas;

// Declare any non-default types here with import statements

interface IConnectionService {
    /**
     * Demonstrates some basic types that you can use as parameters
     * and return values in AIDL.
     */
    void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat,
            double aDouble, String aString);
    int getStatus();
    void setSocket(String ip);
    void connect();
    void disconnect();
    void send();
    void receive();
}
