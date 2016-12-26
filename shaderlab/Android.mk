INNER_SAVED_LOCAL_PATH := $(LOCAL_PATH)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := shaderlab

LOCAL_C_INCLUDES := \
	${LUA_SRC_PATH} \
	${SM_SRC_PATH} \
	${EJOY2D_SRC_PATH} \
	${DS_SRC_PATH} \
	${CU_SRC_PATH} \

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH) -name "*.cpp" -print)) \
	lua_wrap_sl.c \

include $(BUILD_STATIC_LIBRARY)	

LOCAL_PATH := $(INNER_SAVED_LOCAL_PATH)