INNER_SAVED_LOCAL_PATH := $(LOCAL_PATH)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := polymesh

LOCAL_C_INCLUDES := \
	${DS_SRC_PATH} \
	${CU_SRC_PATH} \

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH) -name "*.cpp" -print)) \

include $(BUILD_STATIC_LIBRARY)	

LOCAL_PATH := $(INNER_SAVED_LOCAL_PATH)