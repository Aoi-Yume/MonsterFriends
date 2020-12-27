package com.aoiyume.Game;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Typeface;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;

import com.aoiyume.monsterfriends.MainActivity;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static android.content.Context.WINDOW_SERVICE;

/**
 * Created by 葵ユメ on 2018/09/08.
 */

public class Utility {
    static {
        System.loadLibrary("aoiyume_pch-lib");
    }

    static Object GetAssetManager(){
        return MainActivity.GetContext().getAssets();
    }

    static Object LoadImage(String resPath) {
        InputStream stream = null;
        try {
            stream = MainActivity.GetContext().getAssets().open(resPath);
        } catch (IOException e) {
            Log.d("AoiyumeLib", e.getMessage());
        }
        Bitmap image = BitmapFactory.decodeStream(stream);
        ByteBuffer buffer = ByteBuffer.allocateDirect(image.getByteCount());
        buffer.order(ByteOrder.nativeOrder());
        image.copyPixelsToBuffer(buffer);
        image.recycle();
        return buffer;
    }

    static int[] GetImageSize(String resPath) {
        int[] size = new int[2];
        InputStream stream = null;
        try {
            stream = MainActivity.GetContext().getAssets().open(resPath);
        } catch (IOException e) {
            Log.d("AoiyumeLib", e.getMessage());
            return size;
        }
        Bitmap image = BitmapFactory.decodeStream(stream);
        size[0] = image.getWidth();
        size[1] = image.getHeight();
        image.recycle();
        return size;
    }

    static float[] GetTextImageSize(String text, int fontSize)
    {
        Paint p = getTextPaint(text, fontSize);
        float[] fontWidth = new float[text.length()];
        p.getTextWidths(text, 0, text.length(), fontWidth);

        float fSumWidth = 0.0f;
        for(int i = 0; i < fontWidth.length; ++i){ fSumWidth += fontWidth[i]; }

        int nTexWidth = 2;
        int nTexHeight = 2;
        while(nTexWidth < fSumWidth){ nTexWidth *= 2; }
        nTexHeight = nTexWidth;
        //Log.d("Kanika Magurona", "ImageSize:[" + nTexWidth + "," + nTexHeight + "]");
        float[] ret = new float[3];
        ret[0] = (float)nTexWidth;
        ret[1] = (float)nTexHeight;
        ret[2] = fSumWidth;
        return ret;
    }

    static Object CreateFontImage(String text, int fontSize)
    {
        Paint p = getTextPaint(text, fontSize);
        final float[] fImageInfo = GetTextImageSize(text, fontSize);
        final int nTexWidth = (int)fImageInfo[0];
        final int nTexHeight = (int)fImageInfo[1];
        final float fSumWidth = fImageInfo[2];

        Bitmap image = Bitmap.createBitmap(nTexWidth, nTexHeight, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(image);
        canvas.drawText(text, 0, text.length(), ((float)nTexWidth - fSumWidth) * 0.5f, (float)(nTexHeight + fontSize) * 0.5f, p);
        //canvas.drawRect(0, 0, 100, 100, p);

        ByteBuffer buffer = ByteBuffer.allocateDirect(image.getByteCount());
        buffer.order(ByteOrder.nativeOrder());
        image.copyPixelsToBuffer(buffer);
        image.recycle();
        buffer.position(0);
        return buffer;
    }

    static Paint getTextPaint(String text, int fontSize){
        Paint p = new Paint();
        /*
        Typeface typeface = Typeface.createFromAsset(MainActivity.GetContext().getAssets(), "fontPath");
        p.setTypeface(typeface);
        */
        p.setColor(Color.WHITE);
        p.setTextSize(fontSize);
        return p;
    }

    public static Point GetApplicationScreenSize(){
        WindowManager wm = (WindowManager)MainActivity.GetContext().getSystemService(WINDOW_SERVICE);
        Display disp = wm.getDefaultDisplay();
        Point size = new Point();
        disp.getSize(size);
        return  size;
    }

    public static  byte[] ToByteArray(Object obj) {
        byte[] ret = null;
        try {
            ByteArrayOutputStream byteOutStream = new ByteArrayOutputStream();
            ObjectOutputStream objOutStream = new ObjectOutputStream(byteOutStream);
            objOutStream.writeObject(obj);
            objOutStream.close();
            byteOutStream.close();
            ret = byteOutStream.toByteArray();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return ret;
    }

    public static <T> T ToObject(byte[] data){
        T ret = null;
        try {
            ByteArrayInputStream byteInputStream = new ByteArrayInputStream(data);
            ObjectInputStream objInputStream = new ObjectInputStream(byteInputStream);
            ret = (T)objInputStream.readObject();
            objInputStream.close();
            byteInputStream.close();
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }
        return ret;
    }

}
