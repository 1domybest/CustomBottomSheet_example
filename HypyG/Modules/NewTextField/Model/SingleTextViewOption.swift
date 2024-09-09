//
//  SingleTextViewOption.swift
//  HypyG
//
//  Created by 온석태 on 8/30/24.
//

import Foundation
import SwiftUI
import UIKit

//var singleTextFieldOption = SingleTextFieldOption(
//    maximunLenght: 10,
//    textColor: .black,
//    font: UIFont.systemFont(ofSize: 14),
//    backgroundColor: .blue.withAlphaComponent(0.2),
//    regexPattern: .onlyEnAndJa,
//    borderColor: .clear,
//    borderStyle: .none,
//    placeholder: "플레이스 홀더",
//    placeholderColor: .brown,
//    leftViewImage: .ad,
//    rightViewImage: .kg,
//    leftImageSize: 20.0,
//    rightImageSize: 20.0,
//    leftPadding: 5,
//    rightPadding: 5,
//    isShowReturnButton: true,
//    isShowClearButton: true,
//    leftViewMode: .always,
//    rightViewMode: .always,
//    onTextChanged: { text in
//        print("textField log: 텍스트 변경 감지 \(text)")
//    },
//    didEndEditing: { text, reson in
//        print("textField log: 텍스트 종료 \(text) 이유 = \(reson)")
//    },
//    didBeginEditing: { text in
//        print("textField log: 텍스트 시작 \(text)")
//    },
//    onFailedRegexPattern: { text, regexPattern in
//        print("textField log: 정규식 실패 text = \(text)  정규식 \(regexPattern)")
//    },
//    onMaximumLength: { text in
//        print("textField log: 최대길이 초과")
//    }
//)

struct SingleTextViewOption {
    var maximunLenght: Int = 0 // 글자 최대길이
    var textColor: UIColor = .black // 텍스트컬러
    var font: UIFont = UIFont(name: Font.SpoqaHanSansNeo.regular.value, size: 14)! // 폰트
    var backgroundColor: UIColor = .clear // 배경색
    var regexPattern: RegexPattern = .none
    
    var borderColor: UIColor = .clear // 보더 색상
    var borderWidth:CGFloat = 0 // 보더 스타일
    var borderCornerRadius:CGFloat = 0 // 보더 스타일
    
    var placeholder:String = "" // 플레이스 홀더
    var placeholderColor:UIColor = .gray // 플레이스 홀더
    
    var horizontalPadding: Double = 10.0 // 왼쪽 텍스트필드와의 패딩
    var verticalPadding: Double = 10.0 // 오른쪽 텍스트필드와의 패딩
    
    var isShowReturnButton: Bool = true // return 버튼 노출유무
    var isShowClearButton: Bool = false // clear (X) 버튼 노출유무
    
    var enableAutoHeight: Bool = false
    var minHeight: CGFloat = 100
    var maxHeight: CGFloat = 200
    
    
    var returnKeyType: UIReturnKeyType = .default
    var keyboardType:UIKeyboardType = .default
    var enablesReturnKeyAutomatically:Bool = false
    var showIndicator:Bool = false
    
    // callback
    var onTextChanged: (String) -> Void = { _ in } // 텍스트 변경되었을때
    var didEndEditing: (String) -> Void  = { _ in } // 조작이 끝났을때
    var didBeginEditing: (String) -> Void = { _ in } // 조작을 시작했을때
    var onFailedRegexPattern: (String, RegexPattern) -> Void  = { _ , _ in } // 조작을 시작했을때
    var onMaximumLength: (String) -> Void = { _ in } // 조작을 시작했을때
    var onChangeHeight: (CGFloat) -> Void = { _ in } // 텍스트 변경되었을때
}
