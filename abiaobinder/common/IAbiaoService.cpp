#define LOG_TAG "IAbiaoService"

#include <utils/Log.h>
#include "IAbiaoService.h"

using namespace android;

class BpAbiaoService : public BpInterface<IAbiaoService>
{
public:
    BpAbiaoService(const sp<IBinder> &impl)
            : BpInterface<IAbiaoService>(impl)
    {

    }

    int32_t getVal()
    {
        Parcel data;
        data.writeInterfaceToken(IAbiaoService::getInterfaceDescriptor());
        Parcel reply;
        remote()->transact(GET_VAL, data, &reply);
        int32_t val = reply.readInt32();
        return val;
    }

    void setVal(int32_t val)
    {
        Parcel data;
        data.writeInterfaceToken(IAbiaoService::getInterfaceDescriptor());
        data.writeInt32(val);
        Parcel reply;
        remote()->transact(SET_VAL, data, &reply);
    }
};

IMPLEMENT_META_INTERFACE(AbiaoService, "com.abiao.service")