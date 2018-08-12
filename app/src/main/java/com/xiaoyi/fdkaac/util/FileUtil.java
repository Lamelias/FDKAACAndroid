package com.xiaoyi.fdkaac.util;

import com.xiaoyi.fdkaac.util.ByteUtil;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by luismaria on 2018/8/11.
 */

public class FileUtil {
    public final static String aacPath = "/sdcard/fdk.aac";
    public final static String pcmPath = "/sdcard/fdk.pcm";

    private String filePath;
    private FileOutputStream fos;

    public FileUtil(String filePath) {
        this.filePath = filePath;
    }

    public void init() {
        try {
            fos = new FileOutputStream(filePath);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    public void write(byte[] data) {
        try {
            if (fos != null) {
                fos.write(data, 0, data.length);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void write(short[] data) {
        try {
            byte[] ab = ByteUtil.shorts2Bytes(data);
            if (fos != null) {
                fos.write(ab, 0, ab.length);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void release() {
        try {
            if (fos != null) {
                fos.flush();
                fos.close();
                fos = null;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
