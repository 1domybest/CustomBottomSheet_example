/*
 * Baund Studio Engine
 *
 * BSETransport.java
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

package com.baund.studioengine;

public class BSETransport {
    static {
        System.loadLibrary("studioengine");
    }
    // 프로젝트의 오디오를 재생합니다.
    public static native void play();
    // 프로젝트의 오디오를 정지합니다.
    public static native void stop();
    // 프로젝트의 오디오재생상태를 반환합니다.
    public static native boolean isPlaying();
    // 프로젝트의 녹음을 활성화합니다.
    public static native void recordOn();
    // 프로젝트의 녹음을 비활성화합니다.
    public static native void recordOff();
    // 프로젝트의 녹음이 활성화되어 있는지를 반환합니다.
    public static native boolean isRecording();
    // 프로젝트의 재생위치를 변경합니다.
    public static native void setPosition(double position);
    // 프로젝트의 재생위치를 반환합니다.
    public static native double getPosition();
}
