/*
 * Baund Studio Engine
 *
 * BSEAudioTrack.cpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#include "Includes/BaundStudioEngine.h"
#include <jni.h>

extern "C"
JNIEXPORT jint JNICALL
Java_com_baund_studioengine_BSEAudioTrack_create(JNIEnv *env, jclass clazz) {
    return _BSEAudioTrack_create();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_remove(JNIEnv *env, jclass clazz, jint trackID) {
    _BSEAudioTrack_remove(trackID);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSEAudioTrack_isValid(JNIEnv *env, jclass clazz, jint trackID) {
    return _BSEAudioTrack_isValid(trackID) != 0;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_muteOn(JNIEnv *env, jclass clazz, jint trackID) {
    _BSEAudioTrack_muteOn(trackID);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_setPanValue(JNIEnv *env, jclass clazz, jint trackID, jdouble percent) {
    _BSEAudioTrack_setPanValue(trackID,percent);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEAudioTrack_getPanValue(JNIEnv *env, jclass clazz, jint trackID) {
    return _BSEAudioTrack_getPanValue(trackID);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_setGainValue(JNIEnv *env, jclass clazz, jint trackID, jdouble percent) {
    _BSEAudioTrack_setGainValue(trackID,percent);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEAudioTrack_getGainValue(JNIEnv *env, jclass clazz, jint trackID) {
    return _BSEAudioTrack_getGainValue(trackID);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_muteOff(JNIEnv *env, jclass clazz, jint trackID) {
    _BSEAudioTrack_muteOff(trackID);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSEAudioTrack_isMute(JNIEnv *env, jclass clazz, jint trackID) {
    return _BSEAudioTrack_isMute(trackID) != 0;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_recordOn(JNIEnv *env, jclass clazz, jint trackID) {
    _BSEAudioTrack_recordOn(trackID);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_recordOff(JNIEnv *env, jclass clazz, jint trackID) {
    _BSEAudioTrack_recordOff(trackID);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSEAudioTrack_isRecording(JNIEnv *env, jclass clazz, jint trackID) {
    return _BSEAudioTrack_isRecording(trackID) != 0;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_monitorOn(JNIEnv *env, jclass clazz, jint trackID) {
    _BSEAudioTrack_monitorOn(trackID);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_monitorOff(JNIEnv *env, jclass clazz, jint trackID) {
    _BSEAudioTrack_monitorOff(trackID);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSEAudioTrack_isMonitoring(JNIEnv *env, jclass clazz, jint trackID) {
    return _BSEAudioTrack_isMonitoring(trackID) != 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_addFX(JNIEnv *env, jclass clazz, jint trackID, jint slot, jstring key) {
    _BSEAudioTrack_addFX(trackID,slot,env->GetStringUTFChars(key,nullptr));
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_baund_studioengine_BSEAudioTrack_getFXCategory(JNIEnv *env, jclass clazz, jint trackID,
                                                        jint slot) {
    return env->NewStringUTF(_BSEAudioTrack_getFXCategory(trackID,slot));
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_baund_studioengine_BSEAudioTrack_getFXName(JNIEnv *env, jclass clazz, jint trackID,
                                                    jint slot) {
    return env->NewStringUTF(_BSEAudioTrack_getFXName(trackID,slot));
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_setFXParameterValue(JNIEnv *env, jclass clazz,
                                                              jint trackID, jint slot,
                                                              jint parameterIndex, jdouble value) {
    _BSEAudioTrack_setFXParameterValue(trackID,slot,parameterIndex,value);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEAudioTrack_getFXParameterValue(JNIEnv *env, jclass clazz,
                                                              jint trackID, jint slot,
                                                              jint parameterIndex) {
    return _BSEAudioTrack_getFXParameterValue(trackID,slot,parameterIndex);
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_baund_studioengine_BSEAudioTrack_getFXParameterName(JNIEnv *env, jclass clazz,
                                                             jint trackID, jint slot, jint parameterIndex) {
    return env->NewStringUTF(_BSEAudioTrack_getFXParameterName(trackID,slot,parameterIndex));
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_activeFX(JNIEnv *env, jclass clazz, jint trackID,
                                                   jint slot) {
    _BSEAudioTrack_activeFX(trackID,slot);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioTrack_bypassFX(JNIEnv *env, jclass clazz, jint trackID,
                                                   jint slot) {
    _BSEAudioTrack_bypassFX(trackID,slot);
}
extern "C"
JNIEXPORT bool JNICALL
Java_com_baund_studioengine_BSEAudioTrack_isActivatedFX(JNIEnv *env, jclass clazz, jint trackID,
                                                        jint slot) {
    return _BSEAudioTrack_isActivatedFX(trackID,slot) != 0;
}
