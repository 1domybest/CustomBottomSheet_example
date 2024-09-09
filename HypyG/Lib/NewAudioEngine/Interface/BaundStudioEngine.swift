/*
 * Baund Studio Engine
 *
 * BaundStudioEngine.swift
 * v1.0.5
 *
 * mega.winter0424@gmail.com
 */

import Foundation
import AVFAudio

typealias BSESize   = Int;
typealias BSEIndex  = Int;
typealias BSECount  = Int;
typealias BSEString = String;

typealias BSEObjectID    = BSEIndex;
typealias BSEPath        = BSEString;
typealias BSESampleValue = Float;
typealias BSESeconds     = Double;
typealias BSEPercent     = Double;
typealias BSEEvent       = Double;
typealias BSETempo       = Double;

typealias BSESampleValueArray = Array<BSESampleValue>;

struct BSEAudioDevice {
    // 오디오기기 사용을 위한 초기화를 진행합니다.
    static func ready() -> Void {
        _BSEAudioDevice_ready()
    }
    // 오디오기기의 작동을 시작합니다.
    static func start() -> Void {
        _BSEAudioDevice_start()
    }
    // 오디오기기의 작동을 정지합니다.
    static func stop() -> Void {
        _BSEAudioDevice_stop()
    }
    // 오디오기기의 설정을 해제합니다.
    static func close() -> Void {
        _BSEAudioDevice_close()
    }
    // 오디오기기의 초기화여부를 반환합니다.
    static func isReady() -> Bool {
        return _BSEAudioDevice_isReady() != 0;
    }
    // 오디오기기가 작동중인지를 반환합니다.
    static func isStart() -> Bool {
        return _BSEAudioDevice_isStart() != 0;
    }
    // 오디오기기의 지연시간을 반환합니다.
    static func getLatency() -> BSESeconds {
        return _BSEAudioDevice_getLatency()
    }
    static let reciever     = "reciever"
    static let speaker      = "speaker"
    static let bluetooth    = "bluetooth"
    static let headphones   = "headphones"
    static let builtinMic   = "builtinMic"
    static let bluetoothMic = "bluetoothMic"
    static let headsetMic   = "headsetMic"
    struct DeviceIO {
        var inputs   = Array<String>()
        var outputs  = Array<String>()
    }
    static func getIOList() -> DeviceIO {
        var deviceIO: DeviceIO = DeviceIO()
        do {
            var canUseHeadset = false;
            var session = AVAudioSession.sharedInstance()
            try session.setCategory(.playAndRecord, mode: .default, options: [.allowBluetooth,.allowBluetoothA2DP])
            deviceIO.inputs.append(reciever)
            deviceIO.inputs.append(speaker)
            if let output = session.currentRoute.outputs.first {
                if (output.portType == AVAudioSession.Port.bluetoothA2DP ||
                    output.portType == AVAudioSession.Port.bluetoothHFP  ||
                    output.portType == AVAudioSession.Port.bluetoothLE) {
                    deviceIO.outputs.append(bluetooth)
                } else if (output.portType == AVAudioSession.Port.headphones) {
                    canUseHeadset = true;
                    deviceIO.outputs.append(headphones)
                }
            }
            if let inputs = session.availableInputs {
                for port in inputs {
                    if (port.portType == AVAudioSession.Port.builtInMic) {
                        if (!canUseHeadset) {
                            deviceIO.inputs.append(builtinMic)
                        }
                    } else if (port.portType == AVAudioSession.Port.bluetoothLE || 
                               port.portType == AVAudioSession.Port.bluetoothHFP) {
                        if (!canUseHeadset) {
                            deviceIO.inputs.append(builtinMic)
                        }
                    } else if (port.portType == AVAudioSession.Port.headsetMic) {
                        deviceIO.inputs.append(headsetMic)
                    } else {
                        continue
                    }
                }
            }
        } catch {
            return deviceIO
        }
        return deviceIO
    }
    static func use(input: String, output: String) -> Bool {
        var session   = AVAudioSession.sharedInstance()
        var options   = [AVAudioSession.CategoryOptions.allowBluetooth,
                         AVAudioSession.CategoryOptions.allowBluetoothA2DP]
        var override  = AVAudioSession.PortOverride.none
        var inputPort = AVAudioSessionPortDescription()
        if (output == headphones) {
            if (input != headsetMic) {
                return false
            }
            if let inputs = session.availableInputs {
                for port in inputs {
                    if (port.portType == AVAudioSession.Port.headsetMic) {
                        do {
                            try session.setCategory(.playAndRecord,
                                                    mode: .default,
                                                    options: [AVAudioSession.CategoryOptions.allowBluetooth,
                                                              AVAudioSession.CategoryOptions.allowBluetoothA2DP])
                            try session.setPreferredInput(port)
                            try session.overrideOutputAudioPort(.none)
                            try session.setActive(true)
                            return true
                        } catch {
                            return false
                        }
                    }
                }
            }
        } else if (output == bluetooth) {
            if (input == headsetMic) {
                return false
            }
            if (input == bluetoothMic) {
                if let inputs = session.availableInputs {
                    for port in inputs {
                        if (port.portType == AVAudioSession.Port.bluetoothLE ||
                            port.portType == AVAudioSession.Port.bluetoothHFP) {
                            do {
                                try session.setCategory(.playAndRecord,
                                                        mode: .default,
                                                        options: [AVAudioSession.CategoryOptions.allowBluetooth,
                                                                  AVAudioSession.CategoryOptions.allowBluetoothA2DP])
                                try session.setPreferredInput(port)
                                try session.overrideOutputAudioPort(.none)
                                try session.setActive(true)
                                return true
                            } catch {
                                return false
                            }
                        }
                    }
                }
            }
            if (input == builtinMic) {
                if let inputs = session.availableInputs {
                    for port in inputs {
                        if (port.portType == AVAudioSession.Port.builtInMic) {
                            do {
                                try session.setCategory(.playAndRecord,
                                                        mode: .default,
                                                        options: AVAudioSession.CategoryOptions.allowBluetoothA2DP)
                                try session.setPreferredInput(port)
                                try session.overrideOutputAudioPort(.none)
                                try session.setActive(true)
                                return true
                            } catch {
                                return false
                            }
                        }
                    }
                }
            }
        } else if (output == speaker) {
            if (input == headsetMic) {
                return false
            }
            if (input == bluetoothMic) {
                return false
            }
            if (input == builtinMic) {
                if let inputs = session.availableInputs {
                    for port in inputs {
                        if (port.portType == AVAudioSession.Port.bluetoothLE ||
                            port.portType == AVAudioSession.Port.bluetoothHFP) {
                            do {
                                try session.setCategory(.playAndRecord,
                                                        mode: .default,
                                                        options: [AVAudioSession.CategoryOptions.allowBluetooth,
                                                                  AVAudioSession.CategoryOptions.allowBluetoothA2DP])
                                try session.setPreferredInput(port)
                                try session.overrideOutputAudioPort(.speaker)
                                try session.setActive(true)
                                return true
                            } catch {
                                return false
                            }
                        }
                    }
                }
            }
        } else if (output == reciever) {
            if (input == headsetMic) {
                return false
            }
            if (input == bluetoothMic) {
                return false
            }
            if (input == builtinMic) {
                if let inputs = session.availableInputs {
                    for port in inputs {
                        if (port.portType == AVAudioSession.Port.bluetoothLE ||
                            port.portType == AVAudioSession.Port.bluetoothHFP) {
                            do {
                                try session.setCategory(.playAndRecord,
                                                        mode: .default,
                                                        options: [AVAudioSession.CategoryOptions.allowBluetooth,
                                                                  AVAudioSession.CategoryOptions.allowBluetoothA2DP])
                                try session.setPreferredInput(port)
                                try session.overrideOutputAudioPort(.none)
                                try session.setActive(true)
                                return true
                            } catch {
                                return false
                            }
                        }
                    }
                }
            }
        }
        return false
    }
}
struct BSEProject {
    // 새로운 프로젝트를 생성하고 생성된 프로젝트의 ID를 반환합니다.
    static func create() -> BSEObjectID {
        return _BSEProject_create()
    }
    // 지정된 프로젝트를 제거하고 다른 프로젝트를 활성화합니다.
    static func remove(projectID: BSEObjectID) -> Void {
        _BSEProject_remove(projectID)
    }
    // 지정된 프로젝트를 활성화합니다.
    static func active(projectID: BSEObjectID) -> Void {
        _BSEProject_active(projectID)
    }
    // 지정된 프로젝트의 활성화 여부를 반환합니다.
    static func isActivated(projectID: BSEObjectID) -> Bool {
        return _BSEProject_isActivated(projectID) != 0
    }
    // 지정된 프로젝트가 유효한지를 반환합니다.
    static func isValid(projectID: BSEObjectID) -> Bool {
        return _BSEProject_isValid(projectID) != 0
    }
    // 활성화된 프로젝트에서 사용할 임시 디렉토리를 설정합니다.
    static func setTempDir(path: BSEPath) -> Void {
        _BSEProject_setTempDir(path)
    }
    // 활성화된 프로젝트에 지정된 임시 디렉토리를 반환합니다.
    static func getTempDir() -> BSEPath {
        return String(cString: _BSEProject_getTempDir()!)
    }
    // 오디오기기 입력에 들어오는 신호의 값을 반환합니다.
    static func getRecValue() -> BSESampleValue {
        return _BSEProject_getRecValue()
    }
    // 프로젝트의 전체 길이를 반환합니다.
    static func getLength() -> BSESeconds {
        return _BSEProject_getLength()
    }
    // 전체 프로젝트를 오디오파일로 출력합니다.
    static func writeToAudioFile() -> BSEPath {
        return String(cString: _BSEProject_writeToAudioFile()!)
    }
    // 기록된 로그를 파일로 출력합니다.
    static func writeLogToFile(path: BSEPath) -> BSEPath {
        return String(cString: _BSEProject_writeLogToFile(path)!)
    }
    // 녹음완료된 아이템이 있는 경우 아이디를 반환합니다.
    static func getRecordItem() -> BSEObjectID {
        return _BSEProject_getRecordItem()
    }
    // 기록중인 녹음버퍼를 비웁니다.
    static func clearRecordBuffer() -> Void {
        _BSEProject_clearRecordBuffer()
    }
    // 프로젝트의 템포를 설정합니다.
    static func setGlobalTempo(tempo: BSETempo) -> Void {
        _BSEProject_setGlobalTempo(tempo)
    }
    // 프로젝트에 설정된 템포를 반환합니다.
    static func getGlobalTempo() -> BSETempo {
        return _BSEProject_getGlobalTempo()
    }
}
struct BSETransport {
    // 프로젝트의 오디오를 재생합니다.
    static func play() -> Void {
        _BSETransport_play()
    }
    // 프로젝트의 오디오를 정지합니다.
    static func stop() -> Void {
        _BSETransport_stop()
    }
    // 프로젝트의 오디오재생상태를 반환합니다.
    static func isPlaying() -> Bool {
        return _BSETransport_isPlaying() != 0
    }
    // 프로젝트의 녹음을 활성화합니다.
    static func recordOn() -> Void {
        _BSETransport_recordOn()
    }
    // 프로젝트의 녹음을 비활성화합니다.
    static func recordOff() -> Void {
        _BSETransport_recordOff()
    }
    // 프로젝트의 녹음이 활성화되어 있는지를 반환합니다.
    static func isRecording() -> Bool {
        return _BSETransport_isRecording() != 0
    }
    // 프로젝트의 재생위치를 변경합니다.
    static func setPosition(position: BSESeconds) -> Void {
        _BSETransport_setPosition(position)
    }
    // 프로젝트의 재생위치를 반환합니다.
    static func getPosition() -> BSESeconds {
        return _BSETransport_getPosition()
    }
}
struct BSEEffect {
    static let none: BSEString = ""
    struct Tone {
        static let bright: BSEString = "tone-bright"
        static let stuffy: BSEString = "tone-stuffy"
        static let thin  : BSEString = "tone-thin"
        static let light : BSEString = "tone-light"
    }
    struct Distortion {
        static let fourBit: BSEString = "distortion-4bit"
        static let lowBit : BSEString = "distortion-lowbit"
    }
    struct Delay {
        static let halfNote        : BSEString = "delay-1/2"
        static let quarterNote     : BSEString = "delay-1/4"
        static let dottedEighthNote: BSEString = "delay-1/6"
        static let eighthNote      : BSEString = "delay-1/8"
    }
    struct Reverb {
        static let shorter: BSEString = "reverb-shorter"
        static let medium : BSEString = "reverb-medium"
        static let longer : BSEString = "reverb-longer"
    }
}
struct BSEAudioTrack {
    // 오디오트랙을 생성합니다.
    static func create() -> BSEObjectID {
        return _BSEAudioTrack_create()
    }
    // 오디오트랙을 제거합니다.
    static func remove(trackID: BSEObjectID) -> Void {
        _BSEAudioTrack_remove(trackID)
    }
    // 오디오트랙이 유효한지를 검사합니다.
    static func isValid(trackID: BSEObjectID) -> Bool {
        return _BSEAudioTrack_isValid(trackID) != 0
    }
    // 트랙의 패닝값을 설정합니다.
    static func setPanValue(trackID: BSEObjectID, value: BSEPercent) -> Void {
        _BSEAudioTrack_setPanValue(trackID,value)
    }
    // 트랙의 패닝값을 반환합니다.
    static func getPanValue(trackID: BSEObjectID) -> BSEPercent {
        return _BSEAudioTrack_getPanValue(trackID)
    }
    // 트랙의 볼륨값을 설정합니다.
    static func setGainValue(trackID: BSEObjectID, value: BSEPercent) -> Void {
        _BSEAudioTrack_setGainValue(trackID,value)
    }
    // 트랙의 볼륨값을 반환합니다.
    static func getGainValue(trackID: BSEObjectID) -> BSEPercent {
        return _BSEAudioTrack_getGainValue(trackID)
    }
    // 트랙의 뮤트를 활성화합니다.
    static func muteOn(trackID: BSEObjectID) -> Void {
        _BSEAudioTrack_muteOn(trackID)
    }
    // 트랙의 뮤트를 비활성화합니다.
    static func muteOff(trackID: BSEObjectID) -> Void {
        _BSEAudioTrack_muteOff(trackID)
    }
    // 트랙의 뮤트 활성화여부를 반환합니다.
    static func isMute(trackID: BSEObjectID) -> Bool {
        return _BSEAudioTrack_isMute(trackID) != 0
    }
    // 트랙의 녹음을 활성화합니다.
    static func recordOn(trackID: BSEObjectID) -> Void {
        _BSEAudioTrack_recordOn(trackID)
    }
    // 트랙의 녹음을 비활성화합니다.
    static func recordOff(trackID: BSEObjectID) -> Void {
        _BSEAudioTrack_recordOff(trackID)
    }
    // 트랙의 녹음 활성화여부를 반환합니다.
    static func isRecording(trackID: BSEObjectID) -> Bool {
        return _BSEAudioTrack_isRecording(trackID) != 0
    }
    // 트랙의 모니터링을 활성화합니다.
    static func monitorOn(trackID: BSEObjectID) -> Void {
        _BSEAudioTrack_monitorOn(trackID)
    }
    // 트랙의 모니터링을 비활성화합니다.
    static func monitorOff(trackID: BSEObjectID) -> Void {
        _BSEAudioTrack_monitorOff(trackID)
    }
    // 트랙의 모니터링 활성화여부를 반환합니다.
    static func isMonitoring(trackID: BSEObjectID) -> Bool {
        return _BSEAudioTrack_isMonitoring(trackID) != 0
    }
    // 트랙의 이펙트 슬롯에 지정된 이펙트를 추가합니다.
    static func addFX(trackID: BSEObjectID, slot: BSEIndex, key: BSEString) -> Void {
        _BSEAudioTrack_addFX(trackID,slot,key)
    }
    // 트랙의 지정된 슬롯의 이펙트의 카테고리를 반환합니다.
    static func getFXCategory(trackID: BSEObjectID, slot: BSEIndex) -> BSEString {
        return String(cString: _BSEAudioTrack_getFXCategory(trackID,slot)!)
    }
    // 트랙의 지정된 슬롯의 이펙트의 이름을 반환합니다.
    static func getFXName(trackID: BSEObjectID, slot: BSEIndex) -> BSEString {
        return String(cString: _BSEAudioTrack_getFXName(trackID,slot)!)
    }
    // 트랙의 지정된 슬롯 이펙트의 설정값을 변경합니다.
    static func setFXParameterValue(trackID: BSEObjectID, slot: BSEIndex, parameterIndex: BSEIndex, value: BSEEvent) -> Void {
        _BSEAudioTrack_setFXParameterValue(trackID,slot,parameterIndex,value)
    }
    // 트랙의 지정된 슬롯 이펙트의 설정값을 반환합니다.
    static func getFXParameterValue(trackID: BSEObjectID, slot: BSEIndex, parameterIndex: BSEIndex) -> BSEEvent {
        return _BSEAudioTrack_getFXParameterValue(trackID, slot, parameterIndex)
    }
    // 트랙의 지정된 슬롯 이펙트의 설정이름을 반환합니다.
    static func getFXParameterName(trackID: BSEObjectID, slot: BSEIndex, parameterIndex: BSEIndex) -> BSEString {
        return String(cString: _BSEAudioTrack_getFXParameterName(trackID,slot,parameterIndex)!)
    }
    // 트랙의 지정된 슬롯 이펙트를 활성화합니다.
    static func activeFX(trackID: BSEObjectID, slot: BSEIndex) -> Void {
        _BSEAudioTrack_activeFX(trackID, slot)
    }
    // 트랙의 지정된 슬롯 이펙트를 바이패스합니다.
    static func bypassFX(trackID: BSEObjectID, slot: BSEIndex) -> Void {
        _BSEAudioTrack_bypassFX(trackID, slot)
    }
    // 트랙의 지정된 슬롯 이펙트의 유효성을 반환합니다.
    static func isActivatedFX(trackID: BSEObjectID, slot: BSEIndex) -> Bool {
        return _BSEAudioTrack_isActivatedFX(trackID, slot) != 0
    }
}
struct BSEAudioItem {
    // 아이템을 추가하고 ObjectID를 반환합니다.
    static func create(trackID: BSEObjectID, position: BSESeconds, filePath: BSEPath) -> BSEObjectID {
        return _BSEAudioItem_create(trackID,position,filePath)
    }
    // 지정된 아이템을 제거합니다.
    static func remove(itemID: BSEObjectID) -> Void {
        _BSEAudioItem_remove(itemID)
    }
    // 지정된 아이템의 위치를 이동합니다.
    static func move(itemID: BSEObjectID, position: BSESeconds, trackID: BSEObjectID) -> Void {
        _BSEAudioItem_move(itemID,position,trackID)
    }
    // 지정된 아이템을 둘로 나눈 후 새로 생성되는 아이템의 ObjectID를 반환합니다.
    static func cut(itemID: BSEObjectID, position: BSESeconds) -> BSEObjectID {
        return _BSEAudioItem_cut(itemID,position)
    }
    // 아이템이 유효한지 여부를 반환합니다.
    static func isValid(itemID: BSEObjectID) -> Bool {
        return _BSEAudioItem_isValid(itemID) != 0
    }
    // 아이템의 위치를 반환합니다.
    static func getPosition(itemID: BSEObjectID) -> BSESeconds {
        return _BSEAudioItem_getPosition(itemID)
    }
    // 아이템의 길이를 반환합니다.
    static func getLength(itemID: BSEObjectID) -> BSESeconds {
        return _BSEAudioItem_getLength(itemID)
    }
    // 아이템에 지정된 오디오 파일의 경로를 가져옵니다.
    static func getAudioFile(itemID: BSEObjectID) -> BSEPath {
        return String(cString: _BSEAudioItem_getAudioFile(itemID)!)
    }
    // 아이템에 지정된 오디오 파일의 시작점을 확인합니다.
    // 예 : -1초 == 파일의 1초후부터 사용, 1초 == 오디오 파일 전에 1초간 공백이 생깁니다.
    static func getOffset(itemID: BSEObjectID) -> BSESeconds {
        return _BSEAudioItem_getOffset(itemID)
    }
    // 아이템의 오디오 파일 시작점을 설정합니다.
    static func setOffset(itemID: BSEObjectID, time: BSESeconds) -> Void {
        _BSEAudioItem_setOffset(itemID,time)
    }
    // 아이템의 왼쪽 시작지점위치를 재설정합니다.
    // 조정되는 위치만큼 아이템의 길이가 늘어나거나 줄어듭니다.
    static func resizeLeftSide(itemID: BSEObjectID, position: BSESeconds) -> Void {
        _BSEAudioItem_resizeLeftSide(itemID,position)
    }
    // 아이템의 길이를 조정합니다.
    static func resizeRightSide(itemID: BSEObjectID, position: BSESeconds) -> Void {
        _BSEAudioItem_resizeRightSide(itemID,position)
    }
    // 아이템의 볼륨을 조정합니다.
    // 0 == 무음, 100 == 원래 크기
    static func setGainValue(itemID: BSEObjectID, gainValue: BSEPercent) -> Void {
        _BSEAudioItem_setGainValue(itemID,gainValue)
    }
    // 아이템에 설정된 볼륨값을 반환합니다.
    static func getGainValue(itemID: BSEObjectID) -> BSEPercent {
        return _BSEAudioItem_getGainValue(itemID)
    }
    // 아이템의 페이드 인 시간을 재설정합니다.
    static func setFadeIn (itemID: BSEObjectID, time: BSESeconds) -> Void {
        _BSEAudioItem_setFadeIn(itemID,time)
    }
    // 아이템의 페이드 아웃 시간을 재설정합니다.
    static func setFadeOut(itemID: BSEObjectID, time: BSESeconds) -> Void {
        _BSEAudioItem_setFadeOut(itemID,time)
    }
    // 아이템에 설정된 페이드 인 시간을 반환합니다.
    static func getFadeIn (itemID: BSEObjectID) -> BSESeconds {
        return _BSEAudioItem_getFadeIn(itemID)
    }
    // 아이템에 설정된 페이드 아웃 시간을 반환합니다.
    static func getFadeOut(itemID: BSEObjectID) -> BSESeconds {
        return _BSEAudioItem_getFadeOut(itemID)
    }
    // 입력된 시간단위에서 가장 큰 샘플값들을 반환합니다.
    static func getPCMSamples(itemID: BSEObjectID, timeUnit: BSESeconds) -> BSESampleValueArray {
        let sampleValueArray = _BSEAudioItem_getPCMSamples(itemID, timeUnit);
        return Array(UnsafeMutableBufferPointer(start: sampleValueArray.data, count: Int(sampleValueArray.size)))
    }
}
