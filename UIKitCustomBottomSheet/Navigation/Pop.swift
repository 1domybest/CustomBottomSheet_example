//
//  SwiftUINavigationController.swift
//  UIkit+SwiftUI
//
//  Created by 온석태 on 12/17/24.
//

import Foundation
import SwiftUI
import UIKit

extension NavigationController {
    ///
    /// 네비게이션 최상단 부모 컨트롤러로 한번에 pop 하는 함수
    ///
    /// - Parameters:
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///
    func popToRoot(animated: Bool) {
        if let navigationController = self.rootViewController {
            navigationController.deinitialize()
            
            navigationController.popToRootViewController(animated: animated)
        }
    }
    
    
    ///
    /// 네비게이션 pop 하는 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func pop() {
        if let navigationController = self.rootViewController {
            let _ = navigationController.popViewController(animated: true)
        }
    }
    
    ///
    /// 네비게이션 pop 하는 함수 + 화면전환 방향 커스텀 지정
    ///
    /// - Parameters:
    ///    - type ( CATransitionSubtype ) : 화면전환 방향
    /// - Returns:
    ///
    func pop(type: CATransitionSubtype) {
        if let navigationController = self.rootViewController {
            let transition = CATransition()
            transition.duration = 0.3
            transition.timingFunction = CAMediaTimingFunction(name: CAMediaTimingFunctionName.easeInEaseOut)
            transition.type = CATransitionType.reveal
            transition.subtype = type
            navigationController.view.layer.add(transition, forKey: kCATransition)
            let _ = navigationController.popViewController(animated: false)
        }
    }
}
