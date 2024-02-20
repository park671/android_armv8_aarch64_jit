package com.park.jitdemo;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;


public class MainActivity extends Activity {

    private static final String TAG = "MainActivity";

    /**
     * arm-v8 aarch64
     * add a + b
     */
    byte[] aarch64AddBinary = {
            // add	w0, w1, w0
            (byte) 0x20,
            (byte) 0x00,
            (byte) 0x00,
            (byte) 0x0b,
            // ret
            (byte) 0xc0,
            (byte) 0x03,
            (byte) 0x5f,
            (byte) 0xd6
    };

    /**
     * arm-v8 aarch64
     * mul a * b
     */
    byte[] aarch64MulBinary = {
            // mul	w0, w1, w0
            (byte) 0x20,
            (byte) 0x7C,
            (byte) 0x00,
            (byte) 0x1B,
            // ret
            (byte) 0xc0,
            (byte) 0x03,
            (byte) 0x5f,
            (byte) 0xd6
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.testButton).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int result = NativeBridge.executeBinary(aarch64AddBinary, 2, 4);
                Log.d(TAG, "add result=" + result);
                result = NativeBridge.executeBinary(aarch64MulBinary, 3, 5);
                Log.d(TAG, "mul result=" + result);
            }
        });

        findViewById(R.id.testVixlButton).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                NativeBridge.testVixl();
            }
        });

        findViewById(R.id.testCompilerButton).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String addSourceCodeInC = """
                        int add(int a, int b) {
                        int c = a * b;
                        return c;
                        }""";
                Log.d(TAG, "result:" + NativeBridge.testCompiler(addSourceCodeInC, 4, 98));
            }
        });
    }

}
