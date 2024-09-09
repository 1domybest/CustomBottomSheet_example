//
//  BottomSheetModel.swift
//  HypyG
//
//  Created by 온석태 on 9/9/24.
//

import Foundation
import SwiftUI
import UIKit

struct CustomUIKitBottomSheetOption {
    var pk: UUID
    var someView: AnyView
    var sheetMode: UIModalPresentationStyle = .custom
    // 핸들 가능 모드 custom, automatic , popover, pageSheet, formSheet
    // 핸들 불가능 모드 currentContext, fullScreen, overCurrentContext, overFullScreen,
    
    var sheetHeight: CGFloat = UIScreen.main.bounds.height / 2
    var dragAvailable: Bool = true
    var availableOutTouchClose: Bool = true
    var showHandler: Bool = true
    
    var backgroundColor: Color = Color.black.opacity(0.5)
    var handlerColor: Color = Color.gray4
    var sheetColor: Color = Color.white0
    var sheetShadowColor: Color = Color.gray
    
    
    var minimumHeight:CGFloat = 100
    var maximumHeight:CGFloat = UIScreen.main.bounds.height
    
    var hasKeyboard: Bool = false
    
    var availableHasHandle: Bool {
        return showHandler && (sheetMode == .custom ||
                               sheetMode == .automatic ||
                               sheetMode == .popover ||
                               sheetMode == .pageSheet ||
                               sheetMode == .formSheet )
    }
}
