package com.park.jitdemo;

public class NativeBridge {

    static {
        System.loadLibrary("jitdemo");
    }

    public static native int executeBinary(byte[] nativeCodes, int a, int b);

    public static native void testVixl();

    public static native int testCompiler(String source_code_c, int a, int b);
}
