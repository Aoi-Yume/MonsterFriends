package com.aoiyume.Game;

/**
 * Created by 葵ユメ on 2018/09/02.
 */

public class Engine {
    public static native void SetTouchInputInfo(int nEvent, float fTouchX, float fTouchY);
    public static native void AccecpNearEndPoint(String endPoint, String Name);
    public static native void ReceiveNearbyData(String endPoint, byte[] data);

    public static native void ActivityStop();
    public static native void ActivityResume();
}
