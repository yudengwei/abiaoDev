package com.abiao.dev;

import android.app.Activity;
import android.os.Bundle;
import android.app.AbiaoManager;
import android.content.Context;
import android.util.Log;
public class MainActivity extends Activity {

    private AbiaoManager mAbiaoManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mAbiaoManager = (AbiaoManager)getSystemService(Context.ABIAO_SERVICE);
        mAbiaoManager.setVal("Hello");
        Log.i("HelloServiceTest", "HelloService TEST getVal = "+mAbiaoManager.getVal());
    }
}