//
//  StreamingManager.swift
//  HypyG
//
//  Created by 온석태 on 12/6/23.
//

import Foundation
import AVFoundation
import RootEncoder
import rtmp
import AVKit

enum BroadcastStatus: String {
    case CREATED =  "CREATED" // 방생성
    case READY =  "READY" // 촬영버튼 클릭
    case LIVE =  "LIVE" // 실제로 방송시작
    case NORMAL_END_WAIT =  "NORMAL_END_WAIT" // 일반 종료
    case EXPLODE_END_WAIT =  "EXPLODE_END_WAIT" // 폭파대기
    
    case NORMAL_END =  "NORMAL_END" // 일반 종료
    case EXPLODE_END =  "EXPLODE_END" // 폭파 대기로 인한 방송종료
    case ADMIN_FORCE_END =  "ADMIN_FORCE_END" // 관리자 강제종료
    case SYSTEM_PROBLEM_END =  "SYSTEM_PROBLEM_END" // 시스템 이상 종료
}

///
/// 라이브 스트리밍 매니저
///
/// - Parameters:
/// - Returns:
///
class StreamingManager {
    var cameraBase: CameraBase!
    var endpoint:String?
    var rtmpUICallback:ConnectCheckerRtmp?
    var tryCount:Int = 0
    
    var currentBitRateIndex:Int = 8
    
    var maximumBitRateIndex:Int = 13
    
    var minimumBitRate:Int = 300000
    
    /**
     기준
     1000b = 1kb
     1000kb = 1mb
     
     50,000b → 0.05 MB
     100,000b → 0.1 MB
     240,000b → 0.23 MB
     300,000b → 0.29 MB
     500,000b → 0.48 MB
     700,000b → 0.67 MB
     900,000b → 0.86 MB
     1,000,000b → 0.95 MB
     1,500,000b → 1.43 MB
     2,000,000b → 1.91 MB
     3,000,000b → 2.86 MB
     4,000,000b → 3.82 MB
     5,000,000b → 4.77 MB
     6,000,000b → 5.72 MB
     
     144p (SD, 저 해상도): 약 0.1 ~ 0.3 Mbps
     240p (SD, 표준 해상도): 약 0.3 ~ 0.7 Mbps
     360p (SD, 표준 해상도): 약 0.4 ~ 1.0 Mbps
     720p (HD, 고화질): 약 1.5 ~ 4.0 Mbps
     1080p (Full HD, 풀 고화질): 약 3.0 ~ 6.0 Mbps
     
     */
    var bitRateList: [Int] = [50000 ,100000, 240000 ,300000, 500000, 700000 ,900000, 1000000, 1500000, 2000000, 3000000, 4000000, 5000000, 6000000]
    
    var maintainCount:Int = 0
    
    var increaseCondition:Int = 10 // 비트레이트 상승 기준 [최소 25 번 [30fps * 10] 혹은 10초 동안 유지 했다면 ]
    var increaseMaxBitRateCondition:Int = 25 // 최고 비트레이트의 상승 기준 [최소 25 번 [30fps * 25] 혹은 25초 동안 현재 비트레이트를 유지했다면]
    var decreaseMaxBitRateCondition:Int = 5 // 최고 비트레이트 유지기준 [ maintainCount 이 decreaseMaxBitRateCondition 보다 아래라면 올라간직후 바로 떨어졌다는 뜻]
    var reconnectCondition:Int = -5 // rtmp 재연결 기준
    
    init(rtmpUICallback: ConnectCheckerRtmp) {
        self.rtmpUICallback = rtmpUICallback
        self.cameraBase = CameraBase(connectCheckerRtmp: self)
    }
    
    deinit {
        
        print("StreamingManager deinit")
    }
    
    func unreference () {
        self.cameraBase.unreference()
        self.rtmpUICallback = nil
        self.cameraBase = nil
    }
    
    ///
    /// 라이브 스트리밍 준비 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///  - Bool : 준비 완료 유무 함수
    ///
    func prepareStreaming () -> Bool {
        if (self.cameraBase.prepareVideo(width: Int32(720), height: Int32(1280), bitRate: bitRateList[currentBitRateIndex]) && self.cameraBase.prepareAudio()) {
            return true
        } else {
            return false
        }
    }
    
    func prepareVideo () -> Bool {
        return self.cameraBase.prepareVideo(width: Int32(720), height: Int32(1280), bitRate: bitRateList[currentBitRateIndex])
    }
    
    ///
    /// 라이브 스트리밍 전용 오디오 시작 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func startAudioEngine () {
        self.cameraBase.startAudioEngine()
    }
    
    ///
    /// 라이브 스트리밍 전용 오디오 정지 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func stopAudioEngine () {
        self.cameraBase.stopAudioEngine()
    }
    
    ///
    /// 라이브 스트리밍 시작함수
    ///
    /// - Parameters:
    ///    - endpoint ( String ) : 라이브 스트리밍 앤드포인드 url
    /// - Returns:
    ///
    func startStreaming (endpoint: String) {
        self.endpoint = endpoint
        self.cameraBase.startStream(endpoint: self.endpoint!)
    }
    
    ///
    /// 라이브 스트리밍 상태 확인 [true 이면 카메라,마이크 등 관련 리소스는 켜져있다는 의미 , false이면 라이브 스트리밍을 다시 시작해야하는 순간임]
    ///
    /// - Parameters:
    ///    - endpoint ( String ) : 라이브 스트리밍 앤드포인드 url
    /// - Returns:
    ///
    func isStreaming () -> Bool {
        return self.cameraBase.isStreaming()
    }
    
    func setMicVolume (volume: Float) {
        self.cameraBase.setMicVolume(volume: volume)
    }
    
    ///
    /// 라이브 스트리밍 시작함수
    ///
    /// - Parameters:
    ///    - endpoint ( String ) : 라이브 스트리밍 앤드포인드 url
    /// - Returns:
    ///
    func reconnectStreaming () {
        _ = self.cameraBase.reTry(delay: 0, reason: "diconneted")
    }
    
    ///
    /// 라이브 스트리밍 정지 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func stopStreaming (onDisConnectRtmp: @escaping () -> Void) {
        self.cameraBase.stopStream(onDisConnectRtmp: {
            onDisConnectRtmp()
        })
    }
    
    public func stopVideoEncoder () {
        cameraBase.stopVideoEncoder()
    }
    
    ///
    /// 비트레이트 계산 함수
    ///
    /// - Parameters:
    ///    - persent ( Double ) : 손실률
    /// - Returns:
    ///
    func calculrateBitRate (persent: Double) {
        //print("손실률 \(persent) 비트레이트 변경 현재 \(bitRateList[currentBitRateIndex]) -> ")
        
        // 손실률이 0 퍼센트 이상
        if persent > 0 {
            // 하락 메커니즘
            if currentBitRateIndex > 0 {
                if maintainCount < decreaseMaxBitRateCondition {
                    // 바로 떨어졌다는 이야기
                    maximumBitRateIndex = currentBitRateIndex - 1
                }
                
                maintainCount = 0
                currentBitRateIndex -= 1
                let newBitRate = bitRateList[currentBitRateIndex]
                self.cameraBase.updateBitRate(bitRate: newBitRate)
                
            } else {
                maintainCount -= 1
                if maintainCount < -5 {
                    self.rtmpUICallback?.onConnectionFailedRtmp(reason: "tooMuchFailed")
                    maintainCount = 0
                    //174
                }
            }
        } else if persent == 0  {
            // 지속 메커니즘
            
            // 지속 10번이 되면 상승 메커니즘
            if maintainCount >= 10 {
                if maximumBitRateIndex == currentBitRateIndex + 1 {
                    // 올라갈 비트레이트가 이미 최대치라면 25번동안 유지후 상승
                    maintainCount += 1
                    
                    if maintainCount >= 25 {
                        // 상승 가능
                        if currentBitRateIndex != bitRateList.count - 1 {
                            currentBitRateIndex += 1
                            
                            let newBitRate = bitRateList[currentBitRateIndex]
                            self.cameraBase.updateBitRate(bitRate: newBitRate)
                        }
                        
                        maximumBitRateIndex = currentBitRateIndex
                        maintainCount = 0
                    }
                } else {
                    // 올라갈 비트레이트가 최대치에 도달하지않았다면 단순 상승
                    if currentBitRateIndex != bitRateList.count - 1 {
                        currentBitRateIndex += 1
                        
                        let newBitRate = bitRateList[currentBitRateIndex]
                        self.cameraBase.updateBitRate(bitRate: newBitRate)
                        
                        maintainCount = 0
                    }
                }
            } else {
                maintainCount += 1
            }
        }
        
        //print("손실률 변경 후 \(bitRateList[currentBitRateIndex]) 현재 유지력 \(maintainCount) ")
    }
}

///
/// 카메라/마이크 에서  매 프레임마다 sampleBuffer 가 넘어오는 callback extension
///
/// - Parameters:
/// - Returns:
///
extension StreamingManager: AppendQueueProtocol {
    
    func appendVideoQueue(pixelBuffer: CVPixelBuffer, time: CMTime) {
        self.cameraBase.getVideoData(pixelBuffer: pixelBuffer, pts: time)
    }
    
    func appendAudioQueue(sampleBuffer: CMSampleBuffer) {}
}


///
/// Rtmp Connection 상태에 따른 콜백 Delegate
///
/// - Parameters:
/// - Returns:
///
extension StreamingManager: ConnectCheckerRtmp {
    func onNewLossRate(persent: Double) {
        self.calculrateBitRate(persent: persent)
    }
    
    func onConnectionSuccessRtmp() {
        print("onConnectionSuccessRtmp")
        self.tryCount = 0
        self.rtmpUICallback?.onConnectionSuccessRtmp()
    }
    
    func onConnectionFailedRtmp(reason: String) {
        print("onConnectionFailedRtmp \(reason)")
        
        self.rtmpUICallback?.onConnectionFailedRtmp(reason: reason)
    }
    
    func onNewBitrateRtmp(bitrate: UInt64) {
        //print("onNewBitrateRtmp", bitrate)
    }
    
    func onDisconnectRtmp() {
        self.rtmpUICallback?.onDisconnectRtmp()
    }
    
    func onAuthErrorRtmp() {
        print("onAuthErrorRtmp")
    }
    
    func onAuthSuccessRtmp() {
        print("onAuthSuccessRtmp")
    }
}
