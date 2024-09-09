//
//  MediaManager.swift
//  HypyG
//
//  Created by 온석태 on 11/25/23.
//

import AVFoundation
import Foundation
import UIKit
import rtmp

///
/// 촬영 모드에대한 enum
///
/// - Parameters:
///    - camera : 기본카메라 모드
///    - streaming : 라이브 스트리밍 모드
/// - Returns:
///
enum MediaMode: String {
    case camera = "camera"
    case externalStreaming = "EXTERNAL"
    case streaming = "INTERNAL"
}

///
/// 미디어 매니저 [촬영,  스트리밍, 기기관리등 총괄하는 매니저]
///
/// - Parameters:
/// - Returns:
///
class MediaManager: NSObject {
    public var cameraManager: CameraManager?
    private var audioManager: AudioMananger?
    private var recordManager:RecordManager?
    private var streamingManager: StreamingManager?
    private var screenWidthPixel:CGFloat
    private var screenHeightPixel:CGFloat
    
    public var currentMediaMode:MediaMode = .camera
    public var isPortrait:Bool = true
    
    var rtmpEndpoint:String?
    
    init(rtmpUICallback: ConnectCheckerRtmp) {
        let screenSize = UIScreen.main.bounds
        let screenWidth = screenSize.width
        let screenHeight = screenSize.height
        let screenScale = UIScreen.main.scale
        
        self.screenWidthPixel = screenWidth * screenScale
        self.screenHeightPixel = screenHeight * screenScale
        self.cameraManager = CameraManager()
        self.audioManager = AudioMananger()
        ScreenSize.shader.setCameraSize(cameraWidthSize: self.screenWidthPixel, cameraHeightSize: self.screenHeightPixel)
        
        
        self.streamingManager = StreamingManager(rtmpUICallback: rtmpUICallback)
        self.recordManager = RecordManager()
        super.init()
    }
    
    deinit {
        print("MediaManager Deinit")
    }
    
    func unreference() {
        self.cameraManager?.stopCamera()
        self.cameraManager?.unreference()
        
        
        self.audioManager?.stopAudio()
        self.audioManager?.unreference()
        
        
        self.streamingManager?.unreference()
        
        self.streamingManager = nil
        self.cameraManager = nil
        self.audioManager = nil
        self.recordManager = nil
    }
    
    ///
    /// 화면 사이즈 변경 함수
    ///
    /// - Parameters:
    ///    - screenWidthPixel ( CGFloat ) : 화면 가로길이
    ///    - screenHeightPixel ( CGFloat ) : 화면 세로길이
    /// - Returns:
    ///
    func switchOrientation (isPortrait: Bool) {
        self.isPortrait = isPortrait
        
        let screenSize = UIScreen.main.bounds
        let screenWidth = screenSize.width
        let screenHeight = screenSize.height
        let screenScale = UIScreen.main.scale
        
        let width = screenWidth * screenScale
        let height = screenHeight * screenScale
        
        if self.isPortrait {
            self.screenWidthPixel = width
            self.screenHeightPixel = height
        } else {
            self.screenWidthPixel = height
            self.screenHeightPixel = width
        }
        ScreenSize.shader.setCameraSize(cameraWidthSize: self.screenWidthPixel, cameraHeightSize: self.screenHeightPixel)
    }
    
    ///
    /// 스트리밍중 카메라 활성화 토글 함수
    ///
    /// - Parameters:
    ///    - isCameraOn ( Bool ) : 카메라 활성화 유무
    /// - Returns:
    ///
    func cameraOnOffToggle (isCameraOn: Bool) {
        if isCameraOn {
            self.cameraManager?.startCameraInternal()
        } else {
            self.cameraManager?.stopCameraInternal(isPause: true)
        }
    }
    
    ///
    /// 미디어 모드 설정 함수 [camera - 기본 카메라 모드/ streaming - 라이브 스트리밍 모드]
    ///
    /// - Parameters:
    ///    - mediaMode ( MediaMode ) : 미디어 모드 [ 카메라 / 라이브 스트리밍 ]
    /// - Returns:
    ///
    func setMediaMode (mediaMode: MediaMode) {
        self.currentMediaMode = mediaMode
        guard let captureSession = self.audioManager?.captureSession else {
            return
        }
        if mediaMode == .camera {
            if !captureSession.isRunning {
                self.audioManager?.startAudio()
                self.audioManager?.setAppendQueueCallback(appendQueueCallback: self.recordManager!)
            }
        } else if mediaMode == .streaming {
            if captureSession.isRunning {
                self.audioManager?.stopAudio()
            }
            self.cameraManager?.setAppendQueueCallback(appendQueueCallback: self.streamingManager!)
        }
    }
    
    ///
    /// 디바이스 기기 활성화 함수
    ///
    /// - Parameters:
    ///    - camera ( Bool ) : 카메라 활성화 유무
    ///    - mic ( Bool ) : 마이크 활성화 유무
    /// - Returns:
    ///
    func startDevice(camera: Bool, mic: Bool, mediaMode: MediaMode) {
        self.currentMediaMode = mediaMode
        if camera {
            cameraManager?.startCamera()
        }
                
        if mic {
            
            do {
                try AVAudioSession.sharedInstance().setCategory(.playAndRecord)
                try AVAudioSession.sharedInstance().setActive(true)
            } catch {
                print("오디오 세션 비활성화 실패: \(error)")
            }
            
            if mediaMode == .camera {
                audioManager?.startAudio()
                audioManager?.setAppendQueueCallback(appendQueueCallback: self.recordManager!)
            } else if mediaMode == .streaming {
                startAudioEngine()
            }
        }
    }
    
    func stopDevice(mediaMode: MediaMode) {
        if mediaMode == .streaming || mediaMode == .externalStreaming {
            stopAudioEngine()
        } else {
            audioManager?.stopAudio()
        }
        cameraManager?.stopCamera()
        
    }
    
    ///
    /// 비디오 촬영 시작함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func startVideoRecord() {
        recordManager?.startVideoRecording()
    }
    
    
    ///
    /// 비디오 촬영 정지 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func stopVideoRecord() {
        recordManager?.stopVideoRecording()
    }
    
    ///
    /// 라이브 스트리밍 준비함수
    ///
    /// - Parameters:
    /// - Returns:
    ///    - Bool : 준비 완료 혹은 실패 유무
    ///
    func prepareLiveStreaming() -> Bool {
        guard let streamingManager = self.streamingManager else {
            return false
        }
        
        return streamingManager.prepareStreaming()
    }
    
    func prepareVideo () -> Bool {
        return ((self.streamingManager?.prepareVideo()) != nil)
    }
    
    ///
    /// 라이브 스트리밍 전용 오디오 시작 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///    - Bool : 준비 완료 혹은 실패 유무
    ///
    func startAudioEngine () {
        streamingManager?.startAudioEngine()
    }
    
    ///
    /// 라이브 스트리밍 전용 오디오 시작 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///    - Bool : 준비 완료 혹은 실패 유무
    ///
    func stopAudioEngine () {
        streamingManager?.stopAudioEngine()
    }
    
    ///
    /// 라이브 스트리밍 시작함수
    ///
    /// - Parameters:
    ///    - rtmpEndpoint ( String ) : 라이브 스트리밍 앤드포인드 url
    /// - Returns:
    ///
    func startLiveStreaming(rtmpEndpoint: String) {
        self.rtmpEndpoint = rtmpEndpoint
        streamingManager?.startStreaming(endpoint: rtmpEndpoint)
    }
    
    ///
    /// 라이브 스트리밍 재연결 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func reconnectLiveStreaming() {
        streamingManager?.reconnectStreaming()
    }
    
    ///
    /// 라이브 스트리밍 정지 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func stopLiveStreaming(onDisConnectRtmp: @escaping () -> Void) {
        streamingManager?.stopStreaming(onDisConnectRtmp: {
            onDisConnectRtmp()
        })
    }
    
    ///
    /// 라이브 스트리밍 상태 확인 [true 이면 카메라,마이크 등 관련 리소스는 켜져있다는 의미 , false이면 라이브 스트리밍을 다시 시작해야하는 순간임]
    ///
    /// - Parameters:
    ///    - endpoint ( String ) : 라이브 스트리밍 앤드포인드 url
    /// - Returns:
    ///
    func isStreaming () -> Bool {
        return ((self.streamingManager?.isStreaming()) != nil)
    }

    
    public func stopVideoEncoder () {
        streamingManager?.stopVideoEncoder()
    }
    
    ///
    /// 사진 촬영 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func takePhoto() {
        recordManager?.takePhoto()
    }
    
    ///
    /// 카메라 줌
    ///
    /// - Parameters:
    ///    - scale ( CGFloat ) : 줌 스케일
    /// - Returns:
    ///
    func setZoom (scale: CGFloat) {
        cameraManager?.handlePinchCamera(scale)
    }
    
    ///
    /// 카메라 방향전환 함수
    ///
    /// - Parameters:
    ///    - isFrontCamera ( Bool ) : 전면카메라 활성화 유무
    /// - Returns:
    ///
    func setCameraPosition (isFrontCamera: Bool) {
        
        var position:AVCaptureDevice.Position = .back
        if isFrontCamera {
            position = .front
        }
        cameraManager?.setPosition(position)
    }
    
    func setMicVolume (volume: Float) {
        self.streamingManager?.setMicVolume(volume: volume)
    }
    
    ///
    /// 플레쉬 유무 확인 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func doseHasFlash () -> Bool {
        return ((cameraManager?.doseHaseTorch()) != nil)
    }
    
    ///
    /// 플레쉬 활성화 함수
    ///
    /// - Parameters:
    ///    - isFlashOn ( Bool ) : 플레쉬 활성화 유무
    /// - Returns:
    ///
    func setFlashOn (isFlashOn: Bool) {
        cameraManager?.toggleTorch(onTorch: isFlashOn)
    }
    
    
    ///
    /// 좌우반전 변경 함수
    ///
    /// - Parameters:
    ///    - isMirrorMode ( Bool ) : 좌우반전 활성화 유무
    /// - Returns:
    ///
    func setMirrorMode (isMirrorMode: Bool) {
        cameraManager?.setMirrorMode(isMirrorMode: isMirrorMode)
    }
}
