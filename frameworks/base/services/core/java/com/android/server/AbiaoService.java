package com.android.server;

import android.content.Context;
import android.os.RemoteException;
import android.util.Slog;
import android.os.IAbiaoService;

public class AbiaoService extends IAbiaoService.Stub {

    static final String TAG = "AbiaoService";
    Context mContext;

    private int mPtr;

    private native int AbiaoServiceInit();
    private native void AbiaoServiceNativeSetVal(String val, int ptr);
    private native String AbiaoServiceNativeGetVal(int ptr);

    public AbiaoService(Context context) {
        Slog.i(TAG, "AbiaoService init");
        mPtr = AbiaoServiceInit();
        Slog.i(TAG, "AbiaoService init result: " + mPtr);
        mContext = context;
    }

    @Override
    public void setVal(String value) throws RemoteException {
        Slog.i(TAG, "set value = " + value);
        AbiaoServiceNativeSetVal(value, mPtr);
    }

    @Override
    public String getVal() throws RemoteException {
        String value = AbiaoServiceNativeGetVal(mPtr);
        Slog.i(TAG, "getValue: " + value);
        return value;
    }
}
