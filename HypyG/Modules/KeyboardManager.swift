//
//  KeyboardManager.swift
//  SwiftUIWithUIKit
//
//  Created by 온석태 on 11/21/23.
//

import Foundation
import UIKit

///
/// 시스템 키보드 관련 관리 클래스
///
/// - Parameters:
/// - Returns:
///
class KeyboardManager {
    /// 콜백
    var callback: KeyboardManangerProtocol?
    
    /// 키보드 활성화 유무
    public var isKeyboardShown: Bool = false
    
    /// 키보드 높이
    public var keyboardHeight: CGFloat?
    
    /// 키보드 감지 활성화 유무 = 여러번 호출되는것을 방지하기위함 Bool 타입의 변수
    public var isMonitoringStart: Bool = false
    
    /// 키보드 활성화 유무
    public var requestKeyboardHidden: Bool = false
    ///
    /// 키보드 이벤트 감지 모니터링 시작
    ///
    /// - Parameters:
    /// - Returns:
    ///
    
    deinit {
        print("KeyboardManager deinit")
    }
    
    func unreference() {
        self.hideKeyboard()
        self.callback = nil
        self.stopMonitoring()
    }
    
    func startMonitoring() {
        if isMonitoringStart { return }
        isMonitoringStart = true
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(keyBoardWillShow(notification:)),
                                               name: UIResponder.keyboardWillShowNotification,
                                               object: nil)
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(keyBoardWillHide(notification:)),
                                               name: UIResponder.keyboardWillHideNotification,
                                               object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(keyboardWillChangeFrame), name: UIResponder.keyboardDidChangeFrameNotification, object: nil)
        print("키보드 모니터링 시작")
    }
    
    
    
    ///
    /// 키보드 이벤트 감지 모니터링 종료
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func stopMonitoring() {
        if !isMonitoringStart { return }
        isMonitoringStart = false
        NotificationCenter.default.removeObserver(
            self,
            name: UIResponder.keyboardWillShowNotification,
            object: nil
        )
        NotificationCenter.default.removeObserver(
            self,
            name: UIResponder.keyboardWillHideNotification,
            object: nil
        )
        NotificationCenter.default.removeObserver(self, name: UIResponder.keyboardWillChangeFrameNotification, object: nil)
        callback = nil
        print("키보드 모니터링 해제")
    }
    
    ///
    /// 프로토콜을 통한 감지 이벤트 콜백등록
    ///
    /// - Parameters:
    ///    - <#파라미터#> ( <#타입#> ) : <#설명#>
    /// - Returns:
    ///
    func setCallback(callback: KeyboardManangerProtocol) {
        startMonitoring()
        self.callback = callback
    }
    
    func removeCallback () {
        self.callback = nil
        stopMonitoring()
    }
    
    
    ///
    /// 키보드 활성화 콜백이벤트
    ///
    /// - Parameters:
    /// - Returns:
    ///
    @objc func keyBoardWillShow(notification: NSNotification) {
        if isKeyboardShown { return }
        isKeyboardShown = true
        let keyboardHeight = (notification.userInfo![UIResponder.keyboardFrameEndUserInfoKey] as! NSValue)
            .cgRectValue.height
        
        self.keyboardHeight = keyboardHeight
        callback?.keyBoardWillShow(notification: notification, keyboardHeight: keyboardHeight)
        
//        if let userInfo = notification.userInfo,
//            let keyboardFrame = userInfo[UIResponder.keyboardFrameEndUserInfoKey] as? CGRect {
//             let keyboardHeight = keyboardFrame.height
//             let animationDuration = userInfo[UIResponder.keyboardAnimationDurationUserInfoKey] as? TimeInterval ?? 0.3 // 기본값 설정
//             
//             print("키보드 애니메이션 듀레이션",animationDuration)
//         }
    }
    
    @objc func keyboardWillChangeFrame(notification: NSNotification) {
        if !isKeyboardShown { return }
        let keyboardHeight = (notification.userInfo![UIResponder.keyboardFrameEndUserInfoKey] as! NSValue)
            .cgRectValue.height
        if self.keyboardHeight != keyboardHeight {
            self.keyboardHeight = keyboardHeight
            callback?.keyBoardWillShow(notification: notification, keyboardHeight: keyboardHeight)
        }
    }


    ///
    /// 키보드 비활성화 콜백이벤트
    ///
    /// - Parameters:
    /// - Returns:
    ///
    @objc func keyBoardWillHide(notification: NSNotification) {
        if !isKeyboardShown { return }
        isKeyboardShown = false
        
        callback?.keyBoardWillHide(notification: notification)
        self.requestKeyboardHidden = false
    }
    
    
    ///
    /// 시스템 키보드 수동으로 여는 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func showKeyboard() {
        UIApplication.shared.sendAction(#selector(UIResponder.becomeFirstResponder),
                                        to: nil,
                                        from: nil,
                                        for: nil)
    }
    
    
    ///
    /// 시스템 키보드 수동으로 닫는 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func hideKeyboard() {
        self.requestKeyboardHidden = true
        UIApplication.shared.sendAction(#selector(UIResponder.resignFirstResponder),
                                        to: nil,
                                        from: nil,
                                        for: nil)
    }
}
