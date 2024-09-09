/*
 * Baund Studio Engine
 *
 * TransportActions.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Actions_TransportActions_hpp
#define BaundStudioEngine_Sources_Actions_TransportActions_hpp

#include "BSETypes.hpp"

#ifdef __cplusplus
extern "C" {
#endif
    
    void _BSETransport_play(void);
    void _BSETransport_stop(void);
    _BSEBool _BSETransport_isPlaying(void);
    
    void _BSETransport_recordOn (void);
    void _BSETransport_recordOff(void);
    _BSEBool _BSETransport_isRecording(void);
    
    void _BSETransport_setPosition(_BSESeconds);
    _BSESeconds _BSETransport_getPosition(void);
    
#ifdef __cplusplus
}
#endif
#endif//BaundStudioEngine_Sources_Actions_TransportActions_hpp
