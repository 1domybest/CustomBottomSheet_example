/*
 * Baund Studio Engine
 *
 * BSEProject.cpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#include "Includes/BaundStudioEngine.h"
#include <jni.h>

extern "C"
JNIEXPORT jint JNICALL
Java_com_baund_studioengine_BSEProject_create(JNIEnv *env, jclass clazz) {
    return _BSEProject_create();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEProject_remove(JNIEnv *env, jclass clazz, jint projectID) {
    _BSEProject_remove(projectID);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEProject_active(JNIEnv *env, jclass clazz, jint projectID) {
    _BSEProject_active(projectID);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSEProject_isActivated(JNIEnv *env, jclass clazz, jint projectID) {
    return _BSEProject_isActivated(projectID) != 0;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSEProject_isValid(JNIEnv *env, jclass clazz, jint projectID) {
    return _BSEProject_isValid(projectID) != 0;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEProject_setTempDir(JNIEnv *env, jclass clazz, jstring path) {
    auto filePath = env->GetStringUTFChars(path, NULL);
    _BSEProject_setTempDir(filePath);
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_baund_studioengine_BSEProject_getTempDir(JNIEnv *env, jclass clazz) {
    return env->NewStringUTF(_BSEProject_getTempDir());
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_baund_studioengine_BSEProject_getRecValue(JNIEnv *env, jclass clazz) {
    return _BSEProject_getRecValue();
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEProject_getLength(JNIEnv *env, jclass clazz) {
    return _BSEProject_getLength();
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_baund_studioengine_BSEProject_writeToAudioFile(JNIEnv *env, jclass clazz) {
    return env->NewStringUTF(_BSEProject_writeToAudioFile());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_baund_studioengine_BSEProject_writeLogToFile(JNIEnv *env, jclass clazz, jstring path) {
    return env->NewStringUTF(_BSEProject_writeLogToFile(env->GetStringUTFChars(path,nullptr)));
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_baund_studioengine_BSEProject_getRecordItem(JNIEnv *env, jclass clazz) {
    return _BSEProject_getRecordItem();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEProject_clearRecordBuffer(JNIEnv *env, jclass clazz) {
    _BSEProject_clearRecordBuffer();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEProject_setGlobalTempo(JNIEnv *env, jclass clazz, jdouble tempo) {
    _BSEProject_setGlobalTempo(tempo);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEProject_getGlobalTempo(JNIEnv *env, jclass clazz) {
    return _BSEProject_getGlobalTempo();
}
