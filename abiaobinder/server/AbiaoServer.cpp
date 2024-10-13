#define LOG_TAG "AbiaoServer"

#include <stdlib.h>
#include <fcntl.h>

#include <utils/Log.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include "../common/IAbiaoService.h"

#define ABIAO_DEVICE_NAME "/dev/abiao"

class AbiaoService : public BnAbiaoService
{
public:
    AbiaoService()
    {
      fd = open(ABIAO_DEVICE_NAME, O_RDWR);
    }
    
    virtual ~AbiaoService()
    {
      if (fd != -1)
      {
        close(fd);
      }
    }

    static void instantiate()
    {
      defaultServiceManager()->addService(String16(ABIAO_SERVICE), new AbiaoService());
    }

    int32_t getVal() override {
      int32_t val = 0;
      if (fd != -1)
      {
        read(fd, &val, sizeof(val));
      }
      return val;
    }

    void setVal(int32_t val) override {
      if (fd != -1) {
        write(fd, &val, sizeof(val));
      }
    }

    status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) override {
        switch(code) {
          case GET_VAL:
          {
            CHECK_INTERFACE(IAbiaoService, data, reply);
            int32_t val = getVal();
            reply->writeInt32(val);
            return NO_ERROR;
          }
          case SET_VAL:
          {
            CHECK_INTERFACE(IAbiaoService, data, reply);
            int32_t val = data.readInt32();
            setVal(val);
            return NO_ERROR;
          }
          default:
          {
            return BBinder::onTransact(code, data, reply, flags);
          }
        }
    }
private:
    int fd;
};

int main(int argc, char** argv) 
{
  AbiaoService::instantiate();
  ProcessState::self()->startThreadPool();
  IPCThreadState::self()->joinThreadPool();
  return 0;
}

