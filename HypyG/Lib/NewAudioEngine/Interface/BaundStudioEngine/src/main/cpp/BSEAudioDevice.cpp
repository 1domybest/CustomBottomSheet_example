/*
 * Baund Studio Engine
 *
 * BSEAudioDevice.cpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#include "Includes/BaundStudioEngine.h"
#include <jni.h>

extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioDevice_ready__(JNIEnv *env, jclass clazz) {
    _BSEAudioDevice_ready();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioDevice_start(JNIEnv *env, jclass clazz) {
    _BSEAudioDevice_start();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioDevice_stop(JNIEnv *env, jclass clazz) {
    _BSEAudioDevice_stop();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioDevice_close(JNIEnv *env, jclass clazz) {
    _BSEAudioDevice_close();
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSEAudioDevice_isReady__(JNIEnv *env, jclass clazz) {
    return _BSEAudioDevice_isReady() != 0;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSEAudioDevice_isStart__(JNIEnv *env, jclass clazz) {
    return _BSEAudioDevice_isStart() != 0;
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEAudioDevice_getLatency(JNIEnv *env, jclass clazz) {
    return _BSEAudioDevice_getLatency();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioDevice_setID(JNIEnv *env, jclass clazz, jint input_id,
                                                 jint output_id) {
    _BSEAudioDevice_setID(input_id,output_id);
}