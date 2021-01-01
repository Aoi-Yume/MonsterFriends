package com.aoiyume.Game;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

import com.aoiyume.Game.Engine;

import java.io.IOException;
import java.util.logging.Logger;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by 葵ユメ on 2018/08/13.
 */

public class GameMainGLSurfaceView extends GLSurfaceView {

    GameMainRender m_Renderer = null;

    public GameMainGLSurfaceView(Context context) {
        super(context, null);
    }

    public GameMainGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);

        setEGLContextClientVersion(2);
        m_Renderer = new GameMainRender();
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setRenderer(m_Renderer);

    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        try {
            m_Renderer.dispose();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        final float fTouchY  = GetRenderHeight() - event.getY();
        Engine.SetTouchInputInfo(event.getAction(), event.getX(), fTouchY);
        return true;
    }

    public static float  GetRenderWidth(){
        return GameMainRender.GetWidth();
    }
    public static float  GetRenderHeight(){
        return GameMainRender.GetHeight();
    }
}

class GameMainRender implements GLSurfaceView.Renderer {

    static {
        System.loadLibrary("aoiyume_pch-lib");
        System.loadLibrary("math-lib");
        System.loadLibrary("algorithm-lib");
        System.loadLibrary("input-lib");
        System.loadLibrary( "net-lib");
        System.loadLibrary("aoiyume_engine-lib");
        System.loadLibrary("jni_main-lib");
        System.loadLibrary("base-lib");
        System.loadLibrary("component-lib");
        System.loadLibrary("entity-lib");
        System.loadLibrary("game-lib");
        System.loadLibrary( "app_param-lib");
        System.loadLibrary("app_entity-lib");
        System.loadLibrary("app_scene-lib");
        System.loadLibrary("boot-lib");
    }

    static private float m_SurfaceWidth = 1920.0f;
    static private float m_SurfaceHeight= 1080.0f;

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        SurfaceCreate();
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int w, int h) {
        SurfaceChanged(w, h);

        m_SurfaceWidth = (float)w;
        m_SurfaceHeight = (float)h;
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        SurfaceDraw();
    }

    void dispose() throws IOException {
       // m_EntityMgr.EntityDestroy();
    }

    public static float GetWidth(){ return  m_SurfaceWidth; }
    public static float GetHeight(){ return  m_SurfaceHeight; }

    private native void SurfaceCreate();
    private native void SurfaceChanged(int w, int h);
    private native void SurfaceDraw();
}