/*
 * Baund Studio Engine
 *
 * ProjectActions.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Actions_ProjectActions_hpp
#define BaundStudioEngine_Sources_Actions_ProjectActions_hpp

#include "BSETypes.hpp"

#ifdef __cplusplus
extern "C" {
#endif
    
    _BSEObjectID _BSEProject_create(void);
    void _BSEProject_remove(_BSEObjectID);
    void _BSEProject_active(_BSEObjectID);
    _BSEBool _BSEProject_isActivated(_BSEObjectID);
    _BSEBool _BSEProject_isValid(_BSEObjectID);
    
    void _BSEProject_setTempDir(_BSEPath);
    _BSEPath _BSEProject_getTempDir(void);
    _BSESampleValue _BSEProject_getRecValue(void);
    _BSESeconds _BSEProject_getLength(void);
    
    _BSEPath _BSEProject_writeToAudioFile(void);
    _BSEPath _BSEProject_writeLogToFile(_BSEPath);
    
    _BSEObjectID _BSEProject_getRecordItem(void);
    void         _BSEProject_clearRecordBuffer(void);
    
    void      _BSEProject_setGlobalTempo(_BSETempo);
    _BSETempo _BSEProject_getGlobalTempo(void);
    
#ifdef __cplusplus
}
#endif
#endif//BaundStudioEngine_Sources_Actions_ProjectActions_hpp
