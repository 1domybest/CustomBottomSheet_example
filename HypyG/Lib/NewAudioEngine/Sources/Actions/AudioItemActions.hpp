/*
 * Baund Studio Engine
 *
 * AudioItemActions.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Actions_AudioItemActions_hpp
#define BaundStudioEngine_Sources_Actions_AudioItemActions_hpp

#include "BSETypes.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    _BSEObjectID _BSEAudioItem_create(_BSEObjectID trackID,_BSESeconds position,_BSEPath filePath);
    void         _BSEAudioItem_remove(_BSEObjectID itemID);
    void         _BSEAudioItem_move  (_BSEObjectID itemID,_BSESeconds position,_BSEObjectID trackID);
    _BSEObjectID _BSEAudioItem_cut   (_BSEObjectID itemID,_BSESeconds position);
    _BSEBool     _BSEAudioItem_isValid(_BSEObjectID itemID);

    _BSESeconds _BSEAudioItem_getPosition (_BSEObjectID itemID);
    _BSESeconds _BSEAudioItem_getLength   (_BSEObjectID itemID);
    _BSEPath    _BSEAudioItem_getAudioFile(_BSEObjectID itemID);

    _BSESeconds _BSEAudioItem_getOffset(_BSEObjectID itemID);
    void _BSEAudioItem_setOffset(_BSEObjectID itemID, _BSESeconds time);
    
    void _BSEAudioItem_resizeLeftSide (_BSEObjectID itemID, _BSESeconds position);
    void _BSEAudioItem_resizeRightSide(_BSEObjectID itemID, _BSESeconds position);
    
    void _BSEAudioItem_setGainValue(_BSEObjectID itemID, _BSEPercent gainValue);
    _BSEPercent _BSEAudioItem_getGainValue(_BSEObjectID itemID);
    
    void _BSEAudioItem_setFadeIn (_BSEObjectID itemID, _BSESeconds time);
    void _BSEAudioItem_setFadeOut(_BSEObjectID itemID, _BSESeconds time);
    _BSESeconds _BSEAudioItem_getFadeIn (_BSEObjectID itemID);
    _BSESeconds _BSEAudioItem_getFadeOut(_BSEObjectID itemID);
    
    _BSESampleValueArray _BSEAudioItem_getPCMSamples(_BSEObjectID itemID, _BSESeconds timeUnit);

#ifdef __cplusplus
}
#endif
#endif//BaundStudioEngine_Sources_Actions_AudioItemActions_hpp
