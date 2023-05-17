LOCAL_PATH := $(call my-dir) 

#先把so编译成module 在编译可执行程序时调用
include $(CLEAR_VARS)
LOCAL_MODULE := jpeg-prebult
LOCAL_SRC_FILES := jpeg/libjpeg.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sqlite-prebult
LOCAL_SRC_FILES := sqlite3/libsqlite.so
include $(PREBUILT_SHARED_LIBRARY)

#编译可执行文件时调用上面编译的so的可执行文件
include $(CLEAR_VARS) 
LOCAL_MODULE := camera4Test_zero_delay
LOCAL_SRC_FILES := main.c\
				   quic/decode.c\
				   quic/identify.c\
				   quic/quirc.c\
				   quic/version_db.c\
				   quic/dbgutil.c\
				   quic/mjpeg.c\
				   quic/qrcodeApi.c\
				   jpeg/yuv_to_jpg.c\
				   jpeg/video_app.c\
				   sqlite3/sqliteTask.c\
				   pca9535/pca9535.c\
				   perDevice/perDevice.c\
				   log/cLog.c\
				   
LOCAL_SHARED_LIBRARIES :=   jpeg-prebult\
							sqlite-prebult\

include $(BUILD_EXECUTABLE)
