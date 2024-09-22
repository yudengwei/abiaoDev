#define LOG_TAG "ABIAO_JNI"

#include "jni.h"
#include "core_jni_helpers.h"

#include <utils/Log.h>
#include <openssl/crypto.h>
#include <hardware/hardware.h>
#include <hardware/abiao.h>

namespace android {

    struct abiao_device* a_device = NULL;

    static inline int abiao_device_open(const hw_module_t* module, struct abiao_device** device) {
        return module->methods->open(module, MODULE_HARDWARE_MODULE_ID, (struct hw_device_t**) device);
    }

    static jint AbiaoServiceInit(JNIEnv* env, jobject /* clazz */) {
        ALOGE("com_android_server_AbiaoService AbiaoServiceInit");
        const hw_module_t *hw_module = NULL;
        if (hw_get_module(MODULE_HARDWARE_MODULE_ID, &hw_module) == 0) {
            ALOGE("ABIAO JNI: abiao stub open");
            if (abiao_device_open(hw_module, &a_device) == 0) {
                ALOGE("ABIAO JNI: abiao device open");
                return 0;
            }
            ALOGE("ABIAO JNI: failed to open abiao device");
            return -1;
        }
        ALOGE("ABIAO JNI: failed to open abiao stub");
        return -1;
    }

    static void AbiaoServiceNativeSetVal(JNIEnv* env, jobject clazz, jstring nativeValue, jint ptr) {
        const char* local_value = (nativeValue) ? env->GetStringUTFChars(nativeValue, NULL) : NULL;
        ALOGE("com_android_server_AbiaoServiceNativeSetVal local_vale = %s", local_value);
        a_device->write_string(a_device, local_value);
        env->ReleaseStringUTFChars(nativeValue, local_value);
    }

    static jstring AbiaoServiceNativeGetVal(JNIEnv* env, jint ptr, jobject clazz) {
        char read_str[50];
        a_device->read_string(a_device, read_str);
        ALOGE("ABIAO JNI: read string %s from abiao device", read_str);
        jstring result = (env)->NewStringUTF(read_str);
        return result;
    }

    static const JNINativeMethod methods[] = {
            {"AbiaoServiceInit", "()I", (void*) AbiaoServiceInit},
            {"AbiaoServiceNativeSetVal", "(Ljava/lang/String;I)V", (void*) AbiaoServiceNativeSetVal},
            {"AbiaoServiceNativeGetVal", "(I)Ljava/lang/String;", (void*) AbiaoServiceNativeGetVal},
    };

    int register_android_server_AbiaoService(JNIEnv* env) {
        return jniRegisterNativeMethods(env, "com/android/server/AbiaoService", methods, NELEM(methods));
    }
}


