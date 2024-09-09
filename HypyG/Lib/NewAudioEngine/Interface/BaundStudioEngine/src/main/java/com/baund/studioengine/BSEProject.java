/*
 * Baund Studio Engine
 *
 * BSEProject.java
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

package com.baund.studioengine;

public class BSEProject {
    static {
        System.loadLibrary("studioengine");
    }
    // 새로운 프로젝트를 생성하고 생성된 프로젝트의 ID를 반환합니다.
    public static native int create();
    // 지정된 프로젝트를 제거하고 다른 프로젝트를 활성화합니다.
    public static native void remove(int projectID);
    // 지정된 프로젝트를 활성화합니다.
    public static native void active(int projectID);
    // 지정된 프로젝트의 활성화 여부를 반환합니다.
    public static native boolean isActivated(int projectID);
    // 지정된 프로젝트가 유효한지를 반환합니다.
    public static native boolean isValid(int projectID);
    // 활성화된 프로젝트에서 사용할 임시 디렉토리를 설정합니다.
    public static native void setTempDir(String path);
    // 활성화된 프로젝트에 지정된 임시 디렉토리를 반환합니다.
    public static native String getTempDir();
    // 오디오기기 입력에 들어오는 신호의 값을 반환합니다.
    public static native float getRecValue();
    // 프로젝트의 전체 길이를 반환합니다.
    public static native double getLength();
    // 전체 프로젝트를 오디오파일로 출력합니다.
    public static native String writeToAudioFile();
    // 기록된 로그를 파일로 출력합니다.
    public static native String writeLogToFile(String path);
    // 녹음완료된 아이템이 있는 경우 아이디를 반환합니다.
    public static native int getRecordItem();
    // 기록중인 녹음버퍼를 비웁니다.
    public static native void clearRecordBuffer();
    // 프로젝트의 템포를 설정합니다.
    public static native void setGlobalTempo(double tempo);
    // 프로젝트에 설정된 템포를 반환합니다.
    public static native double getGlobalTempo();
}
