kernel-abiao 是内核驱动代码， 放到 $(内核源码根目录)/common/drivers/目录下即可

添加一个模块（可以是c可执行程序，也可以是apk）
修改 build/make/target/product/sdk_phone_x86_64.mk
添加一行 PRODUCT_PACKAGES += (模块名)  
