package com.park.jitdemo;

public class NativeBridge {

    static {
        System.loadLibrary("jitdemo");
    }

    public static native int executeBinary(byte[] nativeCodes, int a, int b);

    public static native void testVixl();
}
