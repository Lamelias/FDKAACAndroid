/* DO NOT EDIT THIS FILE - it is machine generated */
/* Header for class com_xiaoyi_fdkaac_FDKCodec */

#ifndef _Included_com_xiaoyi_fdkaac_FDKCodec
#define _Included_com_xiaoyi_fdkaac_FDKCodec

#include <jni.h>
#include <string>
#include "aacenc_lib.h"
#include <android/log.h>
#include <aacdecoder_lib.h>

#define  LOG_TAG    "fdkcodec"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_xiaoyi_fdkaac_FDKCodec
 * Method:    init
 * Signature: (Lcom/xiaoyi/fdkaac/AudioCodecSetting;)V
 */
JNIEXPORT void JNICALL Java_com_xiaoyi_fdkaac_FDKCodec_init
        (JNIEnv *, jobject, jobject);

/*
 * Class:     com_xiaoyi_fdkaac_FDKCodec
 * Method:    encode
 * Signature: ([S)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_xiaoyi_fdkaac_FDKCodec_encode
        (JNIEnv *, jobject, jshortArray);

/*
 * Class:     com_xiaoyi_fdkaac_FDKCodec
 * Method:    release
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_xiaoyi_fdkaac_FDKCodec_release
        (JNIEnv *, jobject);

/*
 * Class:     com_xiaoyi_fdkaac_FDKCodec
 * Method:    initDecorder
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_xiaoyi_fdkaac_FDKCodec_initDecoder
        (JNIEnv *, jobject);

/*
 * Class:     com_xiaoyi_fdkaac_FDKCodec
 * Method:    decode
 * Signature: ([B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_xiaoyi_fdkaac_FDKCodec_decode
        (JNIEnv *, jobject, jbyteArray);

/*
 * Class:     com_xiaoyi_fdkaac_FDKCodec
 * Method:    releaseDecoder
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_xiaoyi_fdkaac_FDKCodec_releaseDecoder
        (JNIEnv *, jobject);

#ifdef __cplusplus
}

#endif
#endif
