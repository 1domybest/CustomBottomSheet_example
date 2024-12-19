//
//  MainViewModel.swift
//  UIKitCustomBottomSheet
//
//  Created by 온석태 on 12/17/24.
//

import Foundation
import SwiftUI
import UIKit
import CustomBottomSheetLibrary

class MainViewModel: ObservableObject {
    var vmPK: UUID = UUID() // viewModel의 고유 pk
    
    init() {
        
    }
    
    
    @MainActor
    func openSwiftUIBottomSheet() {
        let bottomSheetPk = UUID()
        
        
        let view:AnyView = AnyView(
            CustomBottomSheetView(pk: bottomSheetPk)
        )
        
        var bottomSheetOption = CustomUIKitBottomSheetOption(pk: bottomSheetPk, someView: view)
        bottomSheetOption.sheetColor = .white
        bottomSheetOption.handlerColor = .black
        bottomSheetOption.dragAvailable = true
        bottomSheetOption.hasKeyboard = true
        bottomSheetOption.sheetHeight = 604.getHeightScaledDiagonal()
        bottomSheetOption.sheetMode = .custom
        
        self.openBottomSheet(bottomSheetOption: bottomSheetOption)
    }
    
    /// 바텀시트 열기
    @MainActor
    func openBottomSheet(bottomSheetOption: CustomUIKitBottomSheetOption) {
        CustomBottomSheetSingleTone.shared.show(customUIKitBottomSheetOption: bottomSheetOption, sheetPk: bottomSheetOption.pk, viewPk: self.vmPK)
    }
    
    /// 열려있는 모든 바텀시트 닫기
    @MainActor
    func hideAllBottomSheet() {
        CustomBottomSheetSingleTone.shared.hideAll(viewPk: self.vmPK, animated: false)
    }
    
}
