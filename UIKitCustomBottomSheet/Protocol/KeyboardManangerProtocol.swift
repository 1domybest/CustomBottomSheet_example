//
//  KeyboardManangerProtocol.swift
//  SwiftUIWithUIKit
//
//  Created by 온석태 on 11/21/23.
//

import Foundation
import UIKit

///
/// KeyboardManangerProtocol 프로토콜
///
/// - Parameters:
///    - pixelBuffer ( CVPixelBuffer ) : 카메라에서 받아온 프레임 버퍼
///    - time ( CMTime ) : SampleBuffer에 등록된 타임스탬프
/// - Returns:
///
protocol KeyboardManangerProtocol {
    
    ///
    /// 시스템 키보드 활성화됬을때 callback
    ///
    /// - Parameters:
    ///    - notification ( NSNotification ) : 알림정보
    ///    - keyboardHeight ( CGFloat ) : 키보드 높이
    /// - Returns:
    ///
    func keyBoardWillShow(notification: NSNotification, keyboardHeight: CGFloat)
    
    ///
    /// 시스템 키보드 비 활성화됬을때
    ///
    /// - Parameters:
    ///    - notification ( NSNotification ) : 알림정보
    /// - Returns:
    ///
    func keyBoardWillHide(notification: NSNotification)
}
