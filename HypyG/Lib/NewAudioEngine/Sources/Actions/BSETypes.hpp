/*
 * Baund Studio Engine
 *
 * BSETypes.hpp
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#ifndef BaundStudioEngine_Sources_Actions_BSETypes_hpp
#define BaundStudioEngine_Sources_Actions_BSETypes_hpp

#ifdef __cplusplus
extern "C" {
#endif

    typedef int         _BSEBool;
    typedef long        _BSESize;
    typedef long        _BSEIndex;
    typedef long        _BSECount;
    typedef char const* _BSEString;
    
    typedef _BSEIndex  _BSEObjectID;
    typedef _BSEString _BSEPath;
    typedef float      _BSESampleValue;
    typedef double     _BSESeconds;
    typedef double     _BSEPercent;
    typedef double     _BSETempo;
    typedef double     _BSEEvent;
    
    typedef struct _BSESampleValueArray {
        _BSESampleValue* data;
        _BSESize         size;
    } _BSESampleValueArray;

#ifdef __cplusplus
}
#endif

#endif//BaundStudioEngine_Sources_Actions_BSETypes_hpp
