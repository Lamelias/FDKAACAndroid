package com.xiaoyi.fdkaac;

/**
 * Created by Lamelias on 2018/8/10.
 */

public class FDKCodec {

    static {
        System.loadLibrary("fdkcodec");
    }

    public native void init(AudioCodecSetting setting);

    public native byte[] encode(short[] input);

    public native void release();

    public native void initDecoder();

    public native byte[] decode(byte[] input);

    public native void releaseDecoder();

}
