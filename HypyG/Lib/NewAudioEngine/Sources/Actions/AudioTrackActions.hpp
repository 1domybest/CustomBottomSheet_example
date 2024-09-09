/*
 * Baund Studio Engine
 *
 * AudioTrackActions.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Actions_AudioTrackActions_hpp
#define BaundStudioEngine_Sources_Actions_AudioTrackActions_hpp

#include "BSETypes.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    _BSEObjectID _BSEAudioTrack_create(void);
    void _BSEAudioTrack_remove(_BSEObjectID);
    _BSEBool _BSEAudioTrack_isValid(_BSEObjectID);
    
    void        _BSEAudioTrack_setPanValue(_BSEObjectID,_BSEPercent);
    _BSEPercent _BSEAudioTrack_getPanValue(_BSEObjectID);
    void        _BSEAudioTrack_setGainValue(_BSEObjectID,_BSEPercent);
    _BSEPercent _BSEAudioTrack_getGainValue(_BSEObjectID);

    void _BSEAudioTrack_muteOn(_BSEObjectID);
    void _BSEAudioTrack_muteOff(_BSEObjectID);
    _BSEBool _BSEAudioTrack_isMute(_BSEObjectID);
    
    void _BSEAudioTrack_recordOn(_BSEObjectID);
    void _BSEAudioTrack_recordOff(_BSEObjectID);
    _BSEBool _BSEAudioTrack_isRecording(_BSEObjectID);
    
    void _BSEAudioTrack_monitorOn(_BSEObjectID);
    void _BSEAudioTrack_monitorOff(_BSEObjectID);
    _BSEBool _BSEAudioTrack_isMonitoring(_BSEObjectID);

    void _BSEAudioTrack_addFX(_BSEObjectID, _BSEIndex, _BSEString);
    _BSEString _BSEAudioTrack_getFXCategory(_BSEObjectID, _BSEIndex);
    _BSEString _BSEAudioTrack_getFXName(_BSEObjectID, _BSEIndex);
    void _BSEAudioTrack_setFXParameterValue(_BSEObjectID, _BSEIndex, _BSEIndex, _BSEEvent);
    _BSEEvent _BSEAudioTrack_getFXParameterValue(_BSEObjectID, _BSEIndex, _BSEIndex);
    _BSEString _BSEAudioTrack_getFXParameterName(_BSEObjectID, _BSEIndex, _BSEIndex);
    void _BSEAudioTrack_activeFX(_BSEObjectID, _BSEIndex);
    void _BSEAudioTrack_bypassFX(_BSEObjectID, _BSEIndex);
    _BSEBool _BSEAudioTrack_isActivatedFX(_BSEObjectID, _BSEIndex);


#ifdef __cplusplus
}
#endif

#endif//BaundStudioEngine_Sources_Actions_AudioTrackActions_hpp
