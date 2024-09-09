/*
 * Baund Studio Engine
 *
 * iOSAudioFileSupport.mm
 * v1.0.0
 *
 * mega.winter0424@gmail.com
 */

#import <AVFoundation/AVFoundation.h>
#include <string>
#include <fstream>

namespace Baund::StudioEngine::Support {
    auto extractM4A(std::string const& iPath, std::string const& oPath) -> bool {
        if (std::ifstream(oPath).good()) std::remove(oPath.c_str());
        NSString* iFilePath = [NSString stringWithCString:iPath.c_str() encoding:NSUTF8StringEncoding];
        NSString* oFilePath = [NSString stringWithCString:oPath.c_str() encoding:NSUTF8StringEncoding];
        NSURL*    iFileURL  = [NSURL fileURLWithPath:iFilePath];
        AVAsset*      asset       = [AVAsset assetWithURL:iFileURL];
        NSArray*      audioTracks = [asset tracksWithMediaType:AVMediaTypeAudio];
        AVAssetTrack* audioTrack  = [audioTracks firstObject];
        AVAssetExportSession* exportSession = [[AVAssetExportSession alloc] initWithAsset:asset presetName:AVAssetExportPresetPassthrough];
        exportSession.outputFileType = AVFileTypeAppleM4A;
        exportSession.outputURL      = [NSURL fileURLWithPath:oFilePath];
        exportSession.timeRange      = audioTrack.timeRange;
        dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
        [exportSession exportAsynchronouslyWithCompletionHandler:^{
            NSLog(@"Export Session Status: %ld", exportSession.status);
            NSError *error = exportSession.error;
            if (error) {
                NSLog(@"Export Session Error: %@", error);
            }
            dispatch_semaphore_signal(semaphore);
        }];
        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        if      (exportSession.status == AVAssetExportSessionStatusCompleted) goto success;
        else if (exportSession.status == AVAssetExportSessionStatusFailed   ) goto error;
    success:
        return true;
    error:
        return false;
    }
    auto isMP4Video(std::string const& path) -> bool {
        NSString* filePath = [NSString stringWithCString:path.c_str() encoding:NSUTF8StringEncoding];
        BOOL isMPEG4Video = NO;
        NSString* fileExtension = [filePath pathExtension];
        if ([fileExtension isEqualToString:@"mp4"]) {
            isMPEG4Video = YES;
        } else {
            NSData* fileData = [NSData dataWithContentsOfFile:filePath];
            if (fileData != nil && fileData.length >= 4) {
                uint8_t fileHeader[4];
                [fileData getBytes:&fileHeader length:4];
                if (fileHeader[0] == 'f' && fileHeader[1] == 't' && fileHeader[2] == 'y' && fileHeader[3] == 'p') {
                    isMPEG4Video = YES;
                }
            }
        }
        return isMPEG4Video;
    }
}
