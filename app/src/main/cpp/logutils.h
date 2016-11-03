#ifndef LOG_UTILS_H
#define LOG_UTILS_H
#include <android/log.h>

#ifndef LOG_TAG
#define LOG_TAG "NDKLog"
#endif

// Most common
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);

// Less common
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__);
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__);

// Least common
#define LOGU(...) __android_log_print(ANDROID_LOG_UNKNOWN, LOG_TAG, __VA_ARGS__);
#define LOGDE(...) __android_log_print(ANDROID_LOG_DEFAULT, LOG_TAG, __VA_ARGS__);
#define LOGS(...) __android_log_print(ANDROID_LOG_SILENT, LOG_TAG, __VA_ARGS__);
#endif