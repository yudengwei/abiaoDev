#define LOG_TAG "AbiaoClient"

#include <utils/Log.h>
#include <binder/IServiceManager.h>

#include "../common/IAbiaoService.h"

int main()
{
    sp<IBinder> binder = defaultServiceManager()->getService(String16(ABIAO_SERVICE));
    if (binder == NULL) 
    {
        return -1;
    }

    sp<IAbiaoService> service = IAbiaoService::asInterface(binder);
    if (service == NULL)
    {
        return -1;
    }

    printf("Read original value from AbiaoService: \n");

    int32_t val = service->getVal();

    printf("%d.\n", val);

    printf("new val is:\n");
    val += 1;
    service->setVal(val);

    printf("Read new value from AbiaoService: \n");

    val = service->getVal();

    printf("%d.\n", val);
}