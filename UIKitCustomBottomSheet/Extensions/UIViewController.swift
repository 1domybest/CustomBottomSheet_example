//
//  UIViewController.swift
//  UIkit+SwiftUI
//
//  Created by 온석태 on 10/3/24.
//

import Foundation
import UIKit


extension UIViewController {
    
    @objc func shouldPush() -> Bool {
        return true
    }
    
    /// 뷰의 이름 [중복가능]
    var viewName: ViewName? {
        get {
            return objc_getAssociatedObject(self, &viewNameKey) as? ViewName
        }
        set {
            objc_setAssociatedObject(self, &viewNameKey, newValue, .OBJC_ASSOCIATION_RETAIN_NONATOMIC)
        }
    }
    
    /// 뷰의 고유 번호
    var viewPk: UUID? {
        get {
            return objc_getAssociatedObject(self, &viewPkKey) as? UUID
        }
        set {
            objc_setAssociatedObject(self, &viewPkKey, newValue, .OBJC_ASSOCIATION_RETAIN_NONATOMIC)
        }
    }
    
    /// 스와이프 가능여부
    var isSwipeEnable: Bool? {
        get {
            return objc_getAssociatedObject(self, &isSwipeEnableKey) as? Bool
        }
        set {
            objc_setAssociatedObject(self, &isSwipeEnableKey, newValue, .OBJC_ASSOCIATION_RETAIN_NONATOMIC)
        }
    }

    /// 네비게이션 함수중 replace함수로인해 변경되었는지 구분하는 함수
    var isReplaced: Bool? {
        get {
            return objc_getAssociatedObject(self, &isReplacedKey) as? Bool
        }
        set {
            objc_setAssociatedObject(self, &isReplacedKey, newValue, .OBJC_ASSOCIATION_RETAIN_NONATOMIC)
        }
    }

    /// 서버에서 받아오는 고유 pk로 화면의 중복여부를 구분하는 함수
    var contentId: String? {
        get {
            return objc_getAssociatedObject(self, &contentIdKey) as? String
        }
        set {
            objc_setAssociatedObject(self, &contentIdKey, newValue, .OBJC_ASSOCIATION_RETAIN_NONATOMIC)
        }
    }
}
