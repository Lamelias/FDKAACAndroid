package com.xiaoyi.fdkaac;

import android.Manifest;
import android.content.pm.PackageManager;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import com.xiaoyi.fdkaac.util.ByteUtil;
import com.xiaoyi.fdkaac.util.FileUtil;

public class MainActivity extends AppCompatActivity {

    AudioRecordThread audioRecordThread = new AudioRecordThread();

    String[] permissions = new String[]{Manifest.permission.RECORD_AUDIO, Manifest.permission.WRITE_EXTERNAL_STORAGE};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.start).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                requestPermission();
            }
        });

        findViewById(R.id.stop).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopRecord();
            }
        });

        findViewById(R.id.encode).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            }
        });

    }

    private void stopRecord() {
        audioRecordThread.stopThread();
    }

    private void startRecord() {
        audioRecordThread.start();
    }

    private void requestPermission() {

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(permissions[1]) == PackageManager.PERMISSION_GRANTED) {
                startRecord();
            } else {
                requestPermissions(permissions, 1);
            }
        } else {
            startRecord();
        }

    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode == 1) {
            if (grantResults.length > 0 && grantResults[1] == PackageManager.PERMISSION_GRANTED) {
                startRecord();
            }
        }
    }

    class AudioRecordThread extends Thread {

        private String TAG = "AudioRecordThread";

        private int sampleRate = 16000;
        private int channelConfig = AudioFormat.CHANNEL_IN_STEREO;
        private int audioFormat = AudioFormat.ENCODING_PCM_16BIT;

        private int bitRate;
        private int bitsPerSample = 16;
        private int channelCount = 2;

        private boolean isRunning;

        private int bufferSize = 4096;
        private short[] readBuffer;

        private FDKCodec codec;

        @Override
        public void run() {

            int minBufferSize = AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat);
            Log.d(TAG, " AudioRecord min buffer size: " + minBufferSize);

            AudioRecord audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate, channelConfig, audioFormat, minBufferSize);

            if (audioRecord.getState() != AudioRecord.STATE_INITIALIZED) {
                Log.d(TAG, " AudioRecord init failed");
                return;
            }

            audioRecord.startRecording();

            codec = new FDKCodec();
            bitRate = sampleRate * channelCount * 3 / 2;


            Log.d(TAG, " FDKCodec init  sampleRate: " + sampleRate + " channelCount: " + channelCount + " bitRate: " + bitRate);
            codec.init(sampleRate, channelCount, bitRate);

            if (minBufferSize > bufferSize) {
                bufferSize = minBufferSize;
            }

            readBuffer = new short[bufferSize >> 1];
            isRunning = true;

            FileUtil fileUtilAac = new FileUtil(FileUtil.aacPath);
            fileUtilAac.init();

            FileUtil fileUtilPcm = new FileUtil(FileUtil.pcmPath);
            fileUtilPcm.init();

            byte[] encodedData;

            while (isRunning) {
                int readCount = audioRecord.read(readBuffer, 0, bufferSize >> 1);
                Log.d(TAG, "  buffer size: " + (bufferSize >> 1) + " readCount: " + readCount);

                fileUtilPcm.write(readBuffer);

                encodedData = codec.encode(readBuffer);
                if (encodedData != null) {
                    Log.d(TAG, "encodedData: " + ByteUtil.bytes2HexString(encodedData));
                    fileUtilAac.write(encodedData);
                } else {
                    Log.d(TAG, " Encode fail ");
                }

            }

            // flush decode buffer data
            encodedData = codec.encode(null);
            while (encodedData != null) {
                Log.d(TAG, "encodedData: " + encodedData.length);
                fileUtilAac.write(encodedData);
                encodedData = codec.encode(null);
            }

            audioRecord.stop();
            audioRecord.release();
            codec.release();

            fileUtilAac.release();
            fileUtilPcm.release();

        }


        public void stopThread() {
            isRunning = false;
            interrupt();
        }

    }


}
