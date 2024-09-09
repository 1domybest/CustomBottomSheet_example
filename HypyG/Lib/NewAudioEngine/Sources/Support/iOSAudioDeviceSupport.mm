/*
 * Baund Studio Engine
 *
 * iOSAudioDeviceSupport.mm
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#import <Foundation/Foundation.h>
#import <AVFAudio/AVFAudio.h>

#if TARGET_OS_IPHONE

namespace Baund::StudioEngine::Support {
    auto setBufferSize(UInt32 bufferSize) -> bool {
        AVAudioSession* session = [AVAudioSession sharedInstance];
        double duration = bufferSize / [session sampleRate];
        NSError* sessionError = nil;
        [session setPreferredIOBufferDuration:duration error:&sessionError];
        if(sessionError) return false;
        else             return true;
    }
}

#endif//TARGET_OS_IPHONE
