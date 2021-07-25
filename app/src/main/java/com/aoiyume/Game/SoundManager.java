package com.aoiyume.Game;

import android.content.res.AssetFileDescriptor;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.SoundPool;
import android.widget.Toast;

import com.aoiyume.monsterfriends.MainActivity;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by 葵ユメ on 2021/07/23.
 */

public class SoundManager
{
    private static SoundPool m_SEPool = null;

    public static void Initialize()
    {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
            AudioAttributes attributes = new AudioAttributes.Builder().setUsage(AudioAttributes.USAGE_GAME).setContentType(AudioAttributes.CONTENT_TYPE_UNKNOWN).build();
            m_SEPool = new SoundPool.Builder().setAudioAttributes(attributes).setMaxStreams(5).build();
        } else {
            m_SEPool = new SoundPool(10, AudioManager.STREAM_MUSIC, 0);
        }
        m_SEPool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
            @Override
            public void onLoadComplete(SoundPool soundPool, int id, int status) {
                LoadCompleteCallBack(id, status);
            }
        });
    }

    public static void Finalize()
    {
        m_SEPool.release();
    }

    public static int LoadSE(String path)
    {
        int nSoundId = -1;
        try {
            AssetFileDescriptor descriptor = MainActivity.GetContext().getResources().getAssets().openFd(path);
            nSoundId = m_SEPool.load(descriptor, 0);
        } catch(IOException e){
            Toast.makeText(MainActivity.GetContext(), "Sound Load Failed:" + path, Toast.LENGTH_SHORT).show();
            return -1;
        }
        return nSoundId;
    }

    public static void UnLoadSE(int nHandle)
    {
        m_SEPool.unload(nHandle);
    }

    public static void PlaySE(int nHandle, float leftVol, float rightVol, boolean bLoop)
    {
        m_SEPool.play(nHandle, leftVol, rightVol, 0, bLoop ? -1 : 0, 1.0f);
    }

    public static void StopSE(int nHandle)
    {
        m_SEPool.stop(nHandle);
    }

    private static native void LoadCompleteCallBack(int handle, int status);
}
