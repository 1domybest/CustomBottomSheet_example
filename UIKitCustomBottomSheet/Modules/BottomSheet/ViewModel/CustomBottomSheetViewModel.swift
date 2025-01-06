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
    /// 자신을 호출한 부모에서 생성한 자식의 고유 pk
    var pk: UUID
    
    /// 바텀시트에서 사용하는 옵션
    var bottomSheetOption: CustomUIKitBottomSheetOption?
    
    /// 네비게이션
    var navigationController: NavigationController?
    
    /// 커스텀  바텀시트 [키보드 관련 이벤트를 받기위함]
    var customUIKitBottomSheet: CustomUIKitBottomSheet?
    
    /// 키보드 매니저
    var keyboardManager: KeyboardManager?
    
    /// 자기자신이 또다른 바텀시트가 있는지에대한 여부
    var hasChild: Bool = false
    
    /// 키보드관련 변수
    
    /// 마지막 스크롤 오프셋
    var lastOffset: CGFloat = .zero
    /// 위로 스크롤하는지에대한 여부
    var isUpScrolling: Bool = false
    
    /// 마지막 바텀시트의 높이
    @Published var lastHeight: CGFloat = .zero
    /// 현재 바텀시트의 높이
    @Published var extraHeight: CGFloat = .zero
    
    /* 텍스트 필드 */
    
    /// 텍스트 필드 UIView
    var textFieldView: SingleTextFieldContentView?
    /// 바인딩할 제목 텍스트 필드  변수
    @Published var textFieldText:String = ""
    
    /* 텍스트 뷰 */
    
    /// 텍스트 뷰 UIView
    var textViewView: SingleTextViewContentView?
    
    /// 텍스트 뷰 동적 높이
    @Published var textViewHeight:CGFloat = 40
    /// 바인딩할 제목 텍스트 뷰  변수
    @Published var textViewText:String = "" // 웰컴코멘트 텍스트
    
    init(pk: UUID) {
        self.pk = pk
        keyboardManager = KeyboardManager()
        keyboardManager?.setCallback(callback: self)
        self.initializeField()
    }
    
    /// 텍스트 빌드 & 뷰 초기화 함수
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
    
    /// SwiftUI View가 렌더링된후 받아온 네비게이션은 VM에 저장하여 이곳에서도 push, pop이 가능하도록 하기위한 함수
    func setNavigationController(navigationController: NavigationController) {
        self.navigationController = navigationController
    }
    
    /// 추가 시트 오픈 [자식의 자식시트를 위한 함수]
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
    
    /// SwiftUI View가 onViewAppear됬을때 사용할 함수
    func onViewAppear() {
        DispatchQueue.main.async {
            /// 이곳에서 scroll관련 콜백함수를 등록하는 이유는 타이밍이슈로인해 정상적으로 스크롤 딜리게이트가 등록되지않을수도있다
            /// 그렇기에 완전히 render된후에 등록
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
    
    /// SwiftUI View가 dissmiss 됬을시 사용할 함수
    func onViewDisAppear() {
    }
}


/// 키보드 매니저 딜리게이트
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
