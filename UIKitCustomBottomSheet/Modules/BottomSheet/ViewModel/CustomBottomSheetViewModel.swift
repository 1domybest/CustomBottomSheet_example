//
//  CustomBottomSheetViewModel.swift
//  UIKitCustomBottomSheet
//
//  Created by 온석태 on 12/17/24.
//

import Foundation
import SwiftUI
import CustomBottomSheetLibrary
import KeyboardManager

class CustomBottomSheetViewModel: ObservableObject {
    var bottomSheetOption: CustomUIKitBottomSheetOption?
    var navigationController: NavigationController? // 네비게이션
    var customUIKitBottomSheet: CustomUIKitBottomSheet?
    var keyboardManager: KeyboardManager?
    
    var pk: UUID // 자신을 호출한 부모에서 생성한 자식의 고유 pk
    var hasChild: Bool = false // 자기자신이 또다른 바텀시트가 있는지에대한 여부
    
    /// 키보드관련 변수
    var lastOffset: CGFloat = .zero // 마지막 스크롤 오프셋
    var isUpScrolling: Bool = false // 위로 스크롤하는지에대한 여부
    
    
    @Published var lastHeight: CGFloat = .zero // 마지막 바텀시트의 높이
    @Published var extraHeight: CGFloat = .zero // 현재 바텀시트의 높이
    
    @Published var text: String = "" // 텍스트 필드 1
    @Published var text2: String = "" // 텍스트 필드 2
    @Published var text3: String = "" // 텍스트 필드 3
    
    init(pk: UUID) {
        self.pk = pk
        keyboardManager = KeyboardManager()
        keyboardManager?.setCallback(callback: self)
    }
    
    
    func setNavigationController(navigationController: NavigationController) {
        self.navigationController = navigationController
    }
    
    func openSheet() {
        DispatchQueue.main.async {
            let pk = UUID()
            let bottomSheetModel = CustomUIKitBottomSheetOption(pk: pk, someView: AnyView(CustomBottomSheetView(pk: pk)))
            self.bottomSheetOption = bottomSheetModel
            self.bottomSheetOption?.backgroundColor = .blue.opacity(0.5)
            self.bottomSheetOption?.availableOutTouchClose = true
            self.bottomSheetOption?.sheetColor = .red
            self.bottomSheetOption?.hasKeyboard = true
            self.bottomSheetOption?.handlerColor = .black
            self.bottomSheetOption?.dragAvailable = true
            self.bottomSheetOption?.sheetMode = .custom
            self.bottomSheetOption?.sheetHeight = 300
            self.bottomSheetOption?.onDismiss = {
                self.hasChild = false
            }
            
            if let bottomSheetModel = self.bottomSheetOption {
                let pk = bottomSheetModel.pk
                self.hasChild = true
                CustomBottomSheetSingleTone.shared.show(customUIKitBottomSheetOption: bottomSheetModel, sheetPk: pk, viewPk: UUID())
            }
        }
        
    }
    
    func onViewAppear() {
        DispatchQueue.main.async {
            if self.customUIKitBottomSheet == nil {
                self.customUIKitBottomSheet = CustomBottomSheetSingleTone.shared.findBottomSheet(pk: self.pk)
                self.customUIKitBottomSheet?.customUIKitBottomSheetOption?.onScrolling = { [weak self] offset in
                    guard let self = self else { return }
                    if offset > self.lastOffset {
                        // 위로 스크롤 중
                        self.isUpScrolling = true
                    } else if offset < self.lastOffset {
                        // 아래로 스크롤 중
                        self.isUpScrolling = false
                    }
                    
                    self.lastOffset = offset
                }
                
                self.customUIKitBottomSheet?.customUIKitBottomSheetOption?.onEndTouchScrolling = { [weak self] offset, decelerate in
                    guard let self = self else { return }
                    if !self.isUpScrolling {
                        if ((self.keyboardManager?.isKeyboardShown) != nil) {
                            DispatchQueue.main.async {
                                self.keyboardManager?.hideKeyboard()
                            }
                        }
                    }
                }
            }
        }
    }
    
    func onViewDisAppear() {
//        self.unreference()
    }
}


extension CustomBottomSheetViewModel: KeyboardManangerProtocol {
    func keyBoardWillShow(notification: NSNotification, keyboardHeight: CGFloat) {
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.25) {
            self.customUIKitBottomSheet?.scrollToBottom(animated: true)
        }
        
    }
    
    func keyBoardWillHide(notification: NSNotification) {
    }
}
