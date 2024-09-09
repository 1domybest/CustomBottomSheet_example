//
//  CustomTextField.swift
//  HypyG
//
//  Created by 온석태 on 8/30/24.
//

import Foundation
import UIKit
import SnapKit

class SingleTextField: UITextField {
    
    var singleTextFieldOption: SingleTextFieldOption

    
    var leftImageView:UIImageView? = nil
    var rightImageView:UIImageView? = nil

    var imageViewAndPlaceHodlerSpacing: CGFloat = 8.getWidthScaledDiagonal()
    
    init(options: SingleTextFieldOption) {
         self.singleTextFieldOption = options
        super.init(frame: .zero)
         setup()
     }
    
    
    required init?(coder aDecoder: NSCoder) {
        self.singleTextFieldOption = SingleTextFieldOption()
        super.init(coder: aDecoder)
        setup()
    }
    
    deinit {
        self.removeTarget(self, action: #selector(textFieldDidChange(_:)), for: .editingChanged)
    }
    
    func activate() {
        // 텍스트 필드를 첫 번째 응답자로 만들어 포커스를 설정합니다.
        self.becomeFirstResponder()
    }
    
    // 텍스트 필드를 비활성화(포커스 잃기)하는 메서드
    func deactivate() {
        // 텍스트 필드의 첫 번째 응답자 상태를 해제하여 포커스를 제거합니다.
        self.resignFirstResponder()
    }
    
    // Setup method to configure the text field
    private func setup() {
        // Customize the appearance
        
        self.makeSideView()
        
        self.clearButtonMode = self.singleTextFieldOption.claerViewMode
        self.borderStyle = self.singleTextFieldOption.borderStyle
        self.backgroundColor = self.singleTextFieldOption.backgroundColor
        self.textColor = self.singleTextFieldOption.textColor
        self.font = self.singleTextFieldOption.font
        // Add placeholder text
        self.placeholder = self.singleTextFieldOption.placeholder
        self.delegate = self
        
        self.enablesReturnKeyAutomatically = self.singleTextFieldOption.enablesReturnKeyAutomatically
        self.keyboardType = self.singleTextFieldOption.keyboardType
        self.returnKeyType = self.singleTextFieldOption.returnKeyType
        
        let attributes: [NSAttributedString.Key: Any] = [
            .foregroundColor: self.singleTextFieldOption.placeholderColor, // 색상
            .font: self.singleTextFieldOption.font // 폰트와 크기
        ]
        
        self.attributedPlaceholder = NSAttributedString(string: self.singleTextFieldOption.placeholder, attributes: attributes)
        self.addTarget(self, action: #selector(textFieldDidChange(_:)), for: .editingChanged)
        // Optional: Add additional setup such as setting delegates or adding targets
    }
    
    // 텍스트가 변경될 때 호출되는 메서드입니다.
    @objc func textFieldDidChange(_ textField: UITextField) {
        // 현재 텍스트를 가져옵니다.
        var text = textField.text ?? ""
        
        
        // 현재 텍스트를 NSString으로 변환합니다.
        let result = isValidInput(text: text)
        
        self.updateConstraintsForVisibility(isVisible: text.isEmpty)
        
        if result {
            self.singleTextFieldOption.onTextChanged(text)
        } else {
            var sliceText = String(text.prefix(self.singleTextFieldOption.maximunLenght))
            textField.text = sliceText
            self.singleTextFieldOption.onTextChanged(sliceText)
        }
    }
    
    func makeSideView () {
        let leftPaddingView:UIView = UIView()
        
        let rightPaddingView:UIView = UIView()
        
        if self.singleTextFieldOption.leftViewImage != nil {
            
            let leftImageView = UIImageView(image: UIImage(resource: self.singleTextFieldOption.leftViewImage!))
            leftImageView.contentMode = .scaleAspectFit
            
            leftPaddingView.addSubview(leftImageView)
            
            leftImageView.snp.makeConstraints { make in

                make.width.equalTo(self.singleTextFieldOption.leftImageSize)
                make.height.equalTo(self.singleTextFieldOption.leftImageSize)
                make.top.equalToSuperview() // 상단 엣지
                make.leading.equalToSuperview().inset(self.singleTextFieldOption.leftPadding) // 왼쪽 엣지
                make.trailing.equalToSuperview().inset(imageViewAndPlaceHodlerSpacing) // 오른쪽 엣지
                make.bottom.equalToSuperview() // 하단 엣지
            }
            
            self.leftImageView = leftImageView
            
        } else {
            leftPaddingView.frame = CGRect(x: 0.0, y: 0.0, width: self.singleTextFieldOption.leftPadding, height: 0.0)
        }
        
        if self.singleTextFieldOption.rightViewImage != nil {
            
            // Create right view with padding
            let rightImageView = UIImageView(image: UIImage(resource: self.singleTextFieldOption.rightViewImage!))
            rightImageView.contentMode = .scaleAspectFit
            
            rightPaddingView.addSubview(rightImageView)
            
            rightImageView.snp.makeConstraints { make in

                make.width.equalTo(self.singleTextFieldOption.rightImageSize)
                make.height.equalTo(self.singleTextFieldOption.rightImageSize)
                
                make.top.equalToSuperview() // 상단 엣지
                make.trailing.equalToSuperview().inset(self.singleTextFieldOption.rightPadding) // 왼쪽 엣지
                make.leading.equalToSuperview().inset(imageViewAndPlaceHodlerSpacing) // 오른쪽 엣지
                make.bottom.equalToSuperview() // 하단 엣지
            }
            self.rightImageView = rightImageView
        } else {
            rightPaddingView.frame = CGRect(x: 0.0, y: 0.0, width: self.singleTextFieldOption.rightPadding, height: 0.0)
        }
        
        
        self.leftView = leftPaddingView
        self.leftViewMode = .always //self.singleTextFieldOption.leftViewMode
        
        self.rightView = rightPaddingView
        self.rightViewMode = .always
    }
    
    // Example of overriding a UITextField method
    override func becomeFirstResponder() -> Bool {
        // Custom behavior when the text field becomes the first responder
        print("TextField became first responder")
        return super.becomeFirstResponder()
    }
    
    override func resignFirstResponder() -> Bool {
        // Custom behavior when the text field resigns first responder status
        print("TextField resigned first responder \(self.text)")
        return super.resignFirstResponder()
    }
    
    // 정규식을 사용하여 입력 텍스트를 검증합니다.
    private func checkRegexPattern(text: String) -> Bool {
        let predicate = NSPredicate(format: "SELF MATCHES %@", self.singleTextFieldOption.regexPattern.rawValue)
         return predicate.evaluate(with: text)
     }
    
    func checkMixmumLength (text: String) -> Bool {
        if self.singleTextFieldOption.maximunLenght >= text.count {
            return true
        } else {
            self.singleTextFieldOption.onMaximumLength(text)
            return false
        }
    }
    
    func isValidInput (text: String) -> Bool {
        if self.checkMixmumLength(text: text) {
            // 최대길이를 넘지않았을때
            if self.singleTextFieldOption.regexPattern != .none {
                // 정규식이 포함되어있다면
                if self.checkRegexPattern(text: text) {
                    // 정규식 O
                    return true
                } else {
                    // 정규식 X
                    self.singleTextFieldOption.onFailedRegexPattern(text, self.singleTextFieldOption.regexPattern)
                    return false
                }
            } else {
              return true
            }
        } else {
            // 최대길이를 넘었을때
            if self.singleTextFieldOption.maximunLenght > 0 {
                // 제한길이가 존재할때
                return false
            } else {
                if self.singleTextFieldOption.regexPattern != .none {
                    // 정규식이 포함되어있다면
                    if self.checkRegexPattern(text: text) {
                        // 정규식 O
                        return true
                    } else {
                        // 정규식 X
                        self.singleTextFieldOption.onFailedRegexPattern(text, self.singleTextFieldOption.regexPattern)
                        return false
                    }
                } else {
                    return true
                }
            }
        }
    }
    
}

// UITextField Delegate
extension SingleTextField: UITextFieldDelegate {
    
    // 텍스트 필드 활성화 여부
    func textFieldShouldBeginEditing(_ textField: UITextField) -> Bool {
        return true
    }
    
    // 처음 활성화 되었을때
    func textFieldDidBeginEditing(_ textField: UITextField) {
        self.singleTextFieldOption.didBeginEditing(textField.text ?? "")
        print("textFieldDidBeginEditing: \((textField.text) ?? "Empty")")
    }

    // 텍스트 필드의 포커스를 잃기 시작할때 제어할수있음 포커스를 유지하려면 false
    func textFieldShouldEndEditing(_ textField: UITextField) -> Bool {
        return true
    }
    
    // 텍스트 필드의 포커스를 완전히 잃었을때
    func textFieldDidEndEditing(_ textField: UITextField) {
        print("textFieldDidEndEditing: \((textField.text) ?? "Empty")")
    }
    
    // 텍스트 필드의 포커스를 완전히 잃었을때 이유와 함께 반환
    func textFieldDidEndEditing(_ textField: UITextField, reason: UITextField.DidEndEditingReason) {
        self.singleTextFieldOption.didEndEditing(textField.text ?? "", reason)
    }

    func textField(_ textField: UITextField, shouldChangeCharactersIn range: NSRange, replacementString string: String) -> Bool {
        return true
    }
    
    func updateConstraintsForVisibility(isVisible: Bool) {
        if self.singleTextFieldOption.leftViewImage == nil && self.singleTextFieldOption.rightViewImage == nil { return }
        UIView.animate(withDuration: 0.3, animations: {
            if self.leftImageView != nil {
                self.leftImageView?.snp.updateConstraints { make in
                    if isVisible && self.leftImageView?.alpha != 1.0 {
                        self.leftImageView?.alpha = 1.0
                        make.width.equalTo(self.singleTextFieldOption.leftImageSize)
                        make.height.equalTo(self.singleTextFieldOption.leftImageSize)
                        make.trailing.equalToSuperview().inset(self.imageViewAndPlaceHodlerSpacing)
                    } else {
                        self.leftImageView?.alpha = 0.0
                        make.width.equalTo(0)
                        make.height.equalTo(0)
                        make.trailing.equalToSuperview()
                    }
                }
            }
            
            if self.rightImageView != nil {
                self.rightImageView?.snp.updateConstraints { make in
                    if isVisible && self.rightImageView?.alpha != 1.0 {
                        self.rightImageView?.alpha = 1.0
                        make.width.equalTo(self.singleTextFieldOption.rightImageSize)
                        make.height.equalTo(self.singleTextFieldOption.rightImageSize)
                        make.leading.equalToSuperview().inset(self.imageViewAndPlaceHodlerSpacing)
                    } else {
                        self.rightImageView?.alpha = 0.0
                        make.width.equalTo(0)
                        make.height.equalTo(0)
                        make.leading.equalToSuperview()
                    }
                }
            }
            
            
            // 레이아웃을 새로 고칩니다.
            self.layoutIfNeeded()
        })
        
    }
    
    func setText(text: String) {
        self.text = text
        self.updateConstraintsForVisibility(isVisible: text.isEmpty)
        self.singleTextFieldOption.onTextChanged(text)
    }
    
    
    // 사용자가 커서 포인터의 위치를 변경하려고할때 발생되는 이벤트
    func textFieldDidChangeSelection(_ textField: UITextField) {
        
    }
    
    // 텍스트 필드에 clear 버튼 노출 유무를 정함 ( X 버튼 )
    func textFieldShouldClear(_ textField: UITextField) -> Bool {
        return true
    }
    
    // 텍스트 필드에 return 버튼 노출 유무를 정함 ( return 버튼 )
    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        self.singleTextFieldOption.onEnterReturnButton(textField.text ?? "")
        if self.singleTextFieldOption.returnKeyType == .default {
            self.deactivate()
        }
        return true
    }
}
