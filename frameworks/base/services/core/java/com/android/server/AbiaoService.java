package com.android.server;

import android.content.Context;
import android.os.RemoteException;
import android.util.Slog;
import android.os.IAbiaoService;

public class AbiaoService extends IAbiaoService.Stub {

    static final String TAG = "AbiaoService";
    Context mContext;

    public AbiaoService(Context context) {
        Slog.i(TAG, "AbiaoService init");
        mContext = context;
    }

    @Override
    public void setVal(String value) throws RemoteException {
        Slog.i(TAG, "set value = " + value);
    }

    @Override
    public String getVal() throws RemoteException {
        Slog.i(TAG, "getValue: ");
        return "getValue";
    }
}
