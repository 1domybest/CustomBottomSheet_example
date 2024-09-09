/*
 * Baund Studio Engine
 *
 * BSEAudioItem.java
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

package com.baund.studioengine;

public class BSEAudioItem {
    static {
        System.loadLibrary("studioengine");
    }
    static public native int create(int trackID, double position, String filePath);
    static public native void remove(int itemID);
    static public native void move(int itemID, double position, int trackID);
    static public native int cut(int itemID, double position);
    static public native boolean isValid(int itemID);
    static public native double getPosition(int itemID);
    static public native double getLength(int itemID);
    static public native String getAudioFile(int itemID);
    static public native double getOffset(int itemID);
    static public native void setOffset(int itemID, double time);
    static public native void resizeLeftSide(int itemID, double position);
    static public native void resizeRightSide(int itemID, double position);
    static public native void setGainValue(int itemID, double percent);
    static public native double getGainValue(int itemID);
    static public native void setFadeIn(int itemID, double time);
    static public native void setFadeOut(int itemID, double time);
    static public native double getFadeIn(int itemID);
    static public native double getFadeOut(int itemID);
    static public native float[] getPCMSamples(int itemID, double timeUnit);
}
