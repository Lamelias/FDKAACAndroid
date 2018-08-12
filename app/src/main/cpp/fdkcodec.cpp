#include <jni.h>
#include <string>
#include "aacenc_lib.h"
#include <android/log.h>

#define  LOG_TAG    "fdkcodec"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

HANDLE_AACENCODER gAacEncoder;

extern "C"
JNIEXPORT void JNICALL
Java_com_xiaoyi_fdkaac_FDKCodec_init(JNIEnv *env, jobject instance, jint sampleRate,
                                     jint channelCount, jint bitRate) {


    if (aacEncOpen(&gAacEncoder, 0x01, channelCount) != AACENC_OK) {
        LOGD("aacEncOpen failed ");
        return;
    } else {
        LOGD("aacEncOpen pass ");
    }

    CHANNEL_MODE mode;
    switch (channelCount) {
        case 1:
            mode = MODE_1;
            break;
        case 2:
            mode = MODE_2;
            break;
        case 3:
            mode = MODE_1_2;
            break;
        case 4:
            mode = MODE_1_2_1;
            break;
        case 5:
            mode = MODE_1_2_2;
            break;
        case 6:
            mode = MODE_1_2_2_1;
            break;
        default:
            mode = MODE_1;
            break;
    }
    LOGD("AACENC_CHANNELMODE %d", mode);

    aacEncoder_SetParam(gAacEncoder, AACENC_AOT, AOT_AAC_LC);
    aacEncoder_SetParam(gAacEncoder, AACENC_CHANNELMODE, mode);
    aacEncoder_SetParam(gAacEncoder, AACENC_SAMPLERATE, sampleRate);
    aacEncoder_SetParam(gAacEncoder, AACENC_BITRATE, bitRate);
    aacEncoder_SetParam(gAacEncoder, AACENC_BITRATEMODE, 0);
    aacEncoder_SetParam(gAacEncoder, AACENC_TRANSMUX, TT_MP4_ADTS);
    aacEncoder_SetParam(gAacEncoder, AACENC_PROTECTION, 1);


    if (aacEncEncode(gAacEncoder, NULL, NULL, NULL, NULL) != AACENC_OK) {
        LOGD("Init encoder failed");
        return;
    } else {
        LOGD("Init encoder pass");
    }

    AACENC_InfoStruct info = {0};

    LOGD("AACENC_InfoStruct  %p", info);

    if (aacEncInfo(gAacEncoder, &info) != AACENC_OK) {
        LOGD("aacEncInfo failed ");
        return;
    } else {
        LOGD("aacEncInfo pass ");
        LOGD("frameLength: %d ,inputChannels: %d, maxOutBufBytes: %d", info.frameLength,
             info.inputChannels,
             info.maxOutBufBytes);
    }


    LIB_INFO lib_info;
    if (aacEncGetLibInfo(&lib_info) != AACENC_OK) {
        LOGD("aacEncGetLibInfo failed ");
    } else {
        LOGD("aacEncGetLibInfo pass ");
    }

    LOGD("title: %s  versionStr: %s", lib_info.title, lib_info.versionStr);

}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_xiaoyi_fdkaac_FDKCodec_encode(JNIEnv *env, jobject instance, jshortArray input_) {

    jshort *input = NULL;
    int sizeInBuffer = -1;
    if (NULL != input_) {
        input = env->GetShortArrayElements(input_, NULL);
        sizeInBuffer = env->GetArrayLength(input_);
    }

    AACENC_BufDesc inBufDesc = {0};
    AACENC_BufDesc outBufDesc = {0};

    AACENC_InArgs inArgs = {0};
    AACENC_OutArgs outArgs = {0};

    int inIdentifier = IN_AUDIO_DATA;
    int inElSizes = 2;
    void *in_ptr = input;

    inArgs.numInSamples = sizeInBuffer;

    inBufDesc.numBufs = 1;
    inBufDesc.bufferIdentifiers = &inIdentifier;
    inBufDesc.bufs = &in_ptr;
    inBufDesc.bufSizes = &sizeInBuffer;
    inBufDesc.bufElSizes = &inElSizes;

    LOGD("before encode --> numInSamples %d   bufSizes %d", inArgs.numInSamples,
         sizeInBuffer);

    int outBufferSize = 768 * 2;
    int8_t outBuffer[outBufferSize];

    int outIdentifier = OUT_BITSTREAM_DATA;
    int bufElSizes = 1;
    void *out_ptr = outBuffer;

    outBufDesc.numBufs = 1;
    outBufDesc.bufs = &out_ptr;
    outBufDesc.bufferIdentifiers = &outIdentifier;
    outBufDesc.bufSizes = &outBufferSize;
    outBufDesc.bufElSizes = &bufElSizes;

    int code = aacEncEncode(gAacEncoder, &inBufDesc, &outBufDesc, &inArgs, &outArgs);

    if (code == AACENC_ENCODE_EOF) {
        LOGD("Encode finished,  flush");
    } else if (code == AACENC_OK) {
        LOGD("Encode is in processing");
    } else {
        LOGD("Error happened code: %d", code);
    }

    LOGD("after encode --> numOutBytes %d   numInSamples %d", outArgs.numOutBytes,
         outArgs.numInSamples);

    jbyteArray outArray = NULL;
    if (outArgs.numOutBytes != 0) {
        outArray = env->NewByteArray(outArgs.numOutBytes);
        env->SetByteArrayRegion(outArray, 0, outArgs.numOutBytes, outBuffer);
    }

    if (NULL != input_) {
        env->ReleaseShortArrayElements(input_, input, 0);
    }

    return outArray;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xiaoyi_fdkaac_FDKCodec_decode(JNIEnv *env, jobject instance, jbyteArray input_,
                                       jbyteArray output_) {
    jbyte *input = env->GetByteArrayElements(input_, NULL);
    jbyte *output = env->GetByteArrayElements(output_, NULL);

    // TODO

    env->ReleaseByteArrayElements(input_, input, 0);
    env->ReleaseByteArrayElements(output_, output, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xiaoyi_fdkaac_FDKCodec_release(JNIEnv *env, jobject instance) {

    AACENC_ERROR closeCode = aacEncClose(&gAacEncoder);
    if (closeCode != AACENC_OK) {
        LOGD("aacEncClose failed ");
    } else {
        LOGD("aacEncClose pass ");
    }

}