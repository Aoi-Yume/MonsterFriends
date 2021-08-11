package com.aoiyume.Game;

import android.content.res.AssetFileDescriptor;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.MediaParser;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.os.Debug;
import android.provider.MediaStore;
import android.util.Log;
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
    private static MediaPlayer m_CurrentMediaPlayer = null;
    private static MediaPlayer m_NextMediaPlayer = null;
    private static String m_NextBGMPath = null;

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

        m_CurrentMediaPlayer = createMediaPlayer();
    }

    public static void Finalize()
    {
        m_SEPool.release();
        m_CurrentMediaPlayer.stop();
        m_CurrentMediaPlayer.release();
        m_NextMediaPlayer.release();
    }

    public static int LoadSE(String path)
    {
        int nSoundId = -1;
        try {
            AssetFileDescriptor descriptor = MainActivity.GetContext().getResources().getAssets().openFd(path);
            nSoundId = m_SEPool.load(descriptor, 0);
            descriptor.close();
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

    public static void PlayBGM(String path, boolean bLoop)
    {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    AssetFileDescriptor asset = MainActivity.GetContext().getAssets().openFd(path);
                    m_CurrentMediaPlayer.setDataSource(asset.getFileDescriptor(), asset.getStartOffset(), asset.getLength());
                    m_CurrentMediaPlayer.prepare();
                    asset.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                m_CurrentMediaPlayer.start();

                m_CurrentMediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                    @Override
                    public void onCompletion(MediaPlayer mediaPlayer) {
                        m_CurrentMediaPlayer.release();
                        if(m_NextMediaPlayer != null) {
                            m_CurrentMediaPlayer = m_NextMediaPlayer;
                            m_CurrentMediaPlayer.setOnCompletionListener(this);
                            if(m_NextBGMPath != null) {
                                nextMediaPlayerCreate();
                            }
                        }
                    }
                });

                if(bLoop) {
                    m_NextBGMPath = path;
                    nextMediaPlayerCreate();
                }
                else {
                    m_NextBGMPath = null;
                }
            }
        }).start();
    }

    public static void StopBGM()
    {
        m_CurrentMediaPlayer.stop();
    }

    public static boolean IsPlayBGM() { return m_CurrentMediaPlayer.isPlaying(); }

    public static int PlaySE(int nHandle, float leftVol, float rightVol, boolean bLoop)
    {
        return m_SEPool.play(nHandle, leftVol, rightVol, 0, bLoop ? -1 : 0, 1.0f);
    }

    public static void StopSE(int nHandle)
    {
        m_SEPool.stop(nHandle);
    }

    public static void PauseAll()
    {
        m_SEPool.autoPause();
        if(m_CurrentMediaPlayer != null) {
            m_CurrentMediaPlayer.pause();
        }
    }

    public static void ResumeAll()
    {
        m_SEPool.autoResume();
        if(m_CurrentMediaPlayer != null) {
            m_CurrentMediaPlayer.start();
        }
    }

    private static MediaPlayer createMediaPlayer()
    {
        MediaPlayer player = new MediaPlayer();
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
            AudioAttributes.Builder builder = new AudioAttributes.Builder();
            AudioAttributes attr = builder.setContentType(AudioAttributes.CONTENT_TYPE_MUSIC).setUsage(AudioAttributes.USAGE_GAME).build();
            player.setAudioAttributes(attr);
        }
        else {
            player.setAudioStreamType(AudioManager.STREAM_MUSIC);
        }
        return player;
    }

    private static void nextMediaPlayerCreate()
    {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    m_NextMediaPlayer = createMediaPlayer();
                    try {
                        AssetFileDescriptor asset = MainActivity.GetContext().getAssets().openFd(m_NextBGMPath);
                        m_NextMediaPlayer.setDataSource(asset.getFileDescriptor(), asset.getStartOffset(), asset.getLength());
                        m_NextMediaPlayer.prepare();
                        asset.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    m_CurrentMediaPlayer.setNextMediaPlayer(m_NextMediaPlayer);
                }
            }).start();
    }

    private static native void LoadCompleteCallBack(int handle, int status);
}
