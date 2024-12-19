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
import CustomTextFieldLibrary

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
    
    /* 텍스트 필드 */
    var textFieldView: SingleTextFieldContentView?
    @Published var textFieldText:String = "" // 제목 텍스트
    
    /* 텍스트 뷰 */
    var textViewView: SingleTextViewContentView?
    @Published var textViewHeight:CGFloat = 40 // 웰컴코멘트 동적 높이
    @Published var textViewText:String = "" // 웰컴코멘트 텍스트
    

    
    init(pk: UUID) {
        self.pk = pk
        keyboardManager = KeyboardManager()
        keyboardManager?.setCallback(callback: self)
        self.initializeField()
    }
    
    func initializeField () {

        
        var textViewOption = SingleTextViewOption()
        textViewOption.backgroundColor = .clear
        textViewOption.textColor = .white
        textViewOption.placeholder = "텍스트 뷰 플레이스 홀더"
        textViewOption.backgroundColor = .brown
        textViewOption.placeholderColor = .black
        textViewOption.enableAutoHeight = true
        textViewOption.minHeight = 40
        textViewOption.maxHeight = 72
        textViewOption.maximunLenght = 1000
        textViewOption.font = UIFont.systemFont(ofSize: 14)
        textViewOption.horizontalPadding = 14.getWidthScaledDiagonal()
        textViewOption.verticalPadding = 11.getHeightScaledDiagonal()
        textViewOption.onChangeHeight = {[ weak self ]  height in
            guard let self = self else { return }
            self.textViewHeight = height
        }
        
        textViewOption.onTextChanged = {[ weak self ]  text in
            guard let self = self else { return }
            self.textViewText = text
        }
        
        var textFieldOption = SingleTextFieldOption()
        textFieldOption.font = UIFont.systemFont(ofSize: 14)
        textFieldOption.textColor = .white
        textFieldOption.backgroundColor = .blue
        textFieldOption.placeholder = "텍스트 필드 플레이스 홀더"
        textFieldOption.placeholderColor = .black
        textFieldOption.leftPadding = 14.getWidthScaledDiagonal()
        textFieldOption.placeholderColor = .white.withAlphaComponent(0.5)
        textFieldOption.borderStyle = .none
        textFieldOption.onTextChanged = { [ weak self ] text in
            guard let self = self else { return }
            self.textFieldText = text
        }
        
       
        
        self.textViewView = SingleTextViewContentView(singleTextViewOption: textViewOption)
        self.textFieldView = SingleTextFieldContentView(singleTextFieldOption: textFieldOption)
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
        print("키보드 열림")
        print("키보드 의 높이 \(keyboardHeight)")
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.25) {
            self.customUIKitBottomSheet?.scrollToBottom(animated: true)
        }
        
    }
    
    func keyBoardWillHide(notification: NSNotification) {
        print("키보드 닫힘")
    }
}
