/*
 * Baund Studio Engine
 *
 * BSEAudioTrack.java
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

package com.baund.studioengine;

public class BSEAudioTrack {
    static {
        System.loadLibrary("studioengine");
    }
    // 오디오트랙을 생성합니다.
    public static native int create();
    // 오디오트랙을 제거합니다.
    public static native void remove(int trackID);
    // 오디오트랙이 유효한지를 검사합니다.
    public static native boolean isValid(int trackID);
    // 트랙의 패닝값을 설정합니다.
    public static native void setPanValue(int trackID, double percent);
    // 트랙의 패닝값을 반환합니다.
    public static native double getPanValue(int trackID);
    // 트랙의 볼륨값을 설정합니다.
    public static native void setGainValue(int trackID, double percent);
    // 트랙의 볼륨값을 반환합니다.
    public static native double getGainValue(int trackID);
    // 트랙의 뮤트를 활성화합니다.
    public static native void muteOn(int trackID);
    // 트랙의 뮤트를 비활성화합니다.
    public static native void muteOff(int trackID);
    // 트랙의 뮤트 활성화여부를 반환합니다.
    public static native boolean isMute(int trackID);
    // 트랙의 녹음을 활성화합니다.
    public static native void recordOn(int trackID);
    // 트랙의 녹음을 비활성화합니다.
    public static native void recordOff(int trackID);
    // 트랙의 녹음 활성화여부를 반환합니다.
    public static native boolean isRecording(int trackID);
    // 트랙의 모니터링을 활성화합니다.
    public static native void monitorOn(int trackID);
    // 트랙의 모니터링을 비활성화합니다.
    public static native void monitorOff(int trackID);
    // 트랙의 모니터링 활성화여부를 반환합니다.
    public static native boolean isMonitoring(int trackID);
    // 트랙의 이펙트 슬롯에 지정된 이펙트를 추가합니다.
    public static native void addFX(int trackID, int slot, String key);
    // 트랙의 지정된 슬롯의 이펙트의 카테고리를 반환합니다.
    public static native String getFXCategory(int trackID, int slot);
    // 트랙의 지정된 슬롯의 이펙트의 이름을 반환합니다.
    public static native String getFXName(int trackID, int slot);
    // 트랙의 지정된 슬롯 이펙트의 설정값을 변경합니다.
    public static native void setFXParameterValue(int trackID, int slot, int parameterIndex, double value);
    // 트랙의 지정된 슬롯 이펙트의 설정값을 반환합니다.
    public static native double getFXParameterValue(int trackID, int slot, int parameterIndex);
    // 트랙의 지정된 슬롯 이펙트의 설정이름을 반환합니다.
    public static native String getFXParameterName(int trackID, int slot, int parameterIndex);
    // 트랙의 지정된 슬롯 이펙트를 활성화합니다.
    public static native void activeFX(int trackID, int slot);
    // 트랙의 지정된 슬롯 이펙트를 바이패스합니다.
    public static native void bypassFX(int trackID, int slot);
    // 트랙의 지정된 슬롯 이펙트의 유효성을 반환합니다.
    public static native boolean isActivatedFX(int trackID, int slot);
}
