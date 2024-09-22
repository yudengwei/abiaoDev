kernel-abiao 是内核驱动代码， 放到 $(内核源码根目录)/common/drivers/目录下即可

添加一个模块（可以是c可执行程序，也可以是apk）
修改 build/make/target/product/sdk_phone_x86_64.mk
添加一行 PRODUCT_PACKAGES += (模块名)  

将hardware里的文件放到aosp源码对应的目录下，同时修改 源码目录/uild/target/product/sdk_phone_x86_64.mk
	PRODUCT_PACKAGES += mylightref \
                    devtest \
                    abiao.default \

jni:

frameworks/base/services/core/jni/Android.bp
    "com_android_server_abiaoservice.cpp"

frameworks/base/services/core/jni/onload.cpp
    int register_android_server_AbiaoService(JNIEnv* env);
    register_android_server_AbiaoService(env);