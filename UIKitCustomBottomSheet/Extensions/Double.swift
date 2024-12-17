//
//  Double.swift
//  UIKitCustomBottomSheet
//
//  Created by 온석태 on 12/17/24.
//

import Foundation
import UIKit

extension Double {
    
    ///
    /// 디자인에서 사용하는 화면의 가로 사이즈와 현재 개발에 사용하고있는 디바이스의 크기차이를 동적으로 구하는 함수
    ///
    /// - Parameters:
    ///    - basedWidth : 디자이너가 사용하고있는 디바이스의 가로길이
    /// - Returns:
    ///
    func getWidthScaledDiagonal(basedWidth: CGFloat = 393) -> CGFloat {
        let screenWidth = UIScreen.main.bounds.width
        let ratio = CGFloat(self) / basedWidth
        
        return screenWidth * ratio
    }
    
    ///
    /// 디자인에서 사용하는 화면의 세로 사이즈와 현재 개발에 사용하고있는 디바이스의 크기차이를 동적으로 구하는 함수
    ///
    /// - Parameters:
    ///    - basedWidth : 디자이너가 사용하고있는 디바이스의 세로길이
    /// - Returns:
    ///
    func getHeightScaledDiagonal(basedWidth: CGFloat = 852) -> CGFloat {
        let screenHeight = UIScreen.main.bounds.height
        let ratio = CGFloat(self) / basedWidth
        
        return screenHeight * ratio
    }
}
