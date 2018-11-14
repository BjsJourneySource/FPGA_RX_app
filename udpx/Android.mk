
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/include

LOCAL_SRC_FILES := \
    $(LOCAL_PATH)/src/udpx_misc.c \
    $(LOCAL_PATH)/src/udpx_buffer.c \
    $(LOCAL_PATH)/src/udpx_int_ring_buffer.c \
	$(LOCAL_PATH)/src/udpx.c

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

LOCAL_MODULE := udpx

include $(BUILD_SHARED_LIBRARY)

