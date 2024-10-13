#ifndef IABIAOSERVICE_H_
#define IABIAOSERVICE_H_

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

#define ABIAO_SERVICE "com.abiao.service"

using namespace android;

enum 
{
    GET_VAL = IBinder::FIRST_CALL_TRANSACTION,
    SET_VAL
};

class IAbiaoService : public IInterface 
{
public:
    DECLARE_META_INTERFACE(AbiaoService);
    virtual int32_t getVal() = 0;
    virtual void setVal(int32_t val) = 0;
};

class BnAbiaoService : public BnInterface<IAbiaoService>
{
public:
    virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0) = 0;
};

#endif