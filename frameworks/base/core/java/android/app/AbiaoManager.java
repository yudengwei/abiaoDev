package android.app;

import android.content.Context;
import android.os.IAbiaoService;
import android.util.Log;

public class AbiaoManager {
    IAbiaoService mService;

    public AbiaoManager(Context ctx, IAbiaoService service) {
        mService = service;
    }

    public void setVal(String value) {
        try {
            Log.e("AbiaoManager", "AbiaoManager setVal");
            mService.setVal(value);
        } catch (Exception e) {
            Log.e("AbiaoManager", e.toString());
            e.printStackTrace();
        }
    }

    public String getVal() {
        try {
            Log.e("AbiaoManager", "AbiaoManager getVal");
            return mService.getVal();
        } catch (Exception e) {
            Log.e("AbiaoManager", e.toString());
            e.printStackTrace();
        }
        return null;
    }
}
