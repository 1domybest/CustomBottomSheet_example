/*
 * Baund Studio Engine
 *
 * BSEAudioItem.cpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#include "Includes/BaundStudioEngine.h"
#include <jni.h>

extern "C"
JNIEXPORT jint JNICALL
Java_com_baund_studioengine_BSEAudioItem_create(JNIEnv *env, jclass clazz, jint trackID, jdouble position, jstring filePath) {
    return _BSEAudioItem_create(trackID,position,env->GetStringUTFChars(filePath,nullptr));
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioItem_remove(JNIEnv *env, jclass clazz, jint itemID) {
    _BSEAudioItem_remove(itemID);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioItem_move(JNIEnv *env, jclass clazz, jint itemID, jdouble position, jint trackID) {
    _BSEAudioItem_move(itemID,position,trackID);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_baund_studioengine_BSEAudioItem_cut(JNIEnv *env, jclass clazz, jint itemID, jdouble position) {
    return _BSEAudioItem_cut(itemID,position);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_baund_studioengine_BSEAudioItem_isValid(JNIEnv *env, jclass clazz, jint itemID) {
    return _BSEAudioItem_isValid(itemID) != 0;
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEAudioItem_getPosition(JNIEnv *env, jclass clazz, jint itemID) {
    return _BSEAudioItem_getPosition(itemID);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEAudioItem_getLength(JNIEnv *env, jclass clazz, jint itemID) {
    return _BSEAudioItem_getLength(itemID);
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_baund_studioengine_BSEAudioItem_getAudioFile(JNIEnv *env, jclass clazz, jint itemID) {
    return env->NewStringUTF(_BSEAudioItem_getAudioFile(itemID));
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEAudioItem_getOffset(JNIEnv *env, jclass clazz, jint itemID) {
    return _BSEAudioItem_getOffset(itemID);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioItem_setOffset(JNIEnv *env, jclass clazz, jint itemID, jdouble time) {
    _BSEAudioItem_setOffset(itemID,time);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioItem_resizeLeftSide(JNIEnv *env, jclass clazz, jint itemID, jdouble position) {
    _BSEAudioItem_resizeLeftSide(itemID,position);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioItem_resizeRightSide(JNIEnv *env, jclass clazz, jint itemID, jdouble position) {
    _BSEAudioItem_resizeRightSide(itemID,position);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioItem_setGainValue(JNIEnv *env, jclass clazz, jint itemID, jdouble gainValue) {
    _BSEAudioItem_setGainValue(itemID,gainValue);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEAudioItem_getGainValue(JNIEnv *env, jclass clazz, jint itemID) {
    return _BSEAudioItem_getGainValue(itemID);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioItem_setFadeIn(JNIEnv *env, jclass clazz, jint itemID, jdouble time) {
    _BSEAudioItem_setFadeIn(itemID,time);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_baund_studioengine_BSEAudioItem_setFadeOut(JNIEnv *env, jclass clazz, jint itemID, jdouble time) {
    _BSEAudioItem_setFadeOut(itemID,time);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEAudioItem_getFadeIn(JNIEnv *env, jclass clazz, jint itemID) {
    return _BSEAudioItem_getFadeIn(itemID);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_baund_studioengine_BSEAudioItem_getFadeOut(JNIEnv *env, jclass clazz, jint itemID) {
    return _BSEAudioItem_getFadeOut(itemID);
}
extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_baund_studioengine_BSEAudioItem_getPCMSamples(JNIEnv *env, jclass clazz, jint itemID, jdouble timeUnit) {
    auto samples = _BSEAudioItem_getPCMSamples(itemID, timeUnit);
    jfloatArray array = env->NewFloatArray(samples.size);
    env->SetFloatArrayRegion(array,0,samples.size,samples.data);
    return array;
}