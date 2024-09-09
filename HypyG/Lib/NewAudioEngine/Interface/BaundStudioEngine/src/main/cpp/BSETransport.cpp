/*
 * Baund Studio Engine
 *
 * BSETransport.cpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#include "Includes/BaundStudioEngine.h"
#include <jni.h>

extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSETransport_play(JNIEnv *env, jclass clazz) {
    _BSETransport_play();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSETransport_stop(JNIEnv *env, jclass clazz) {
    _BSETransport_stop();
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSETransport_isPlaying(JNIEnv *env, jclass clazz) {
    return _BSETransport_isPlaying() != 0;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSETransport_recordOn(JNIEnv *env, jclass clazz) {
    _BSETransport_recordOn();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSETransport_recordOff(JNIEnv *env, jclass clazz) {
    return _BSETransport_recordOff();
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSETransport_isRecording(JNIEnv *env, jclass clazz) {
    return _BSETransport_isRecording() != 0;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSETransport_setPosition(JNIEnv *env, jclass clazz, jdouble position) {
    _BSETransport_setPosition(position);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSETransport_getPosition(JNIEnv *env, jclass clazz) {
    return _BSETransport_getPosition();
}
