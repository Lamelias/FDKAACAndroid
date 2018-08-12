package com.xiaoyi.fdkaac;

/**
 * Created by Lamelias on 2018/8/10.
 */

public class FDKCodec {

    static {
        System.loadLibrary("fdkcodec");
    }

    public native void init(int sampleRate, int channelCount, int bitRate);

    public native byte[] encode(short[] input);

    public native void decode(byte[] input, byte[] output);

    public native void release();

}
