//
//  SingleTextFieldContentView.swift
//  HypyG
//
//  Created by 온석태 on 8/30/24.
//

import Foundation
import UIKit
import SnapKit

class SingleTextViewContentView: UIView {
    
    var textView: SingleTextView
    
    private var textViewHeightConstraint: Constraint?
    
    init(singleTextViewOption: SingleTextViewOption) {
        textView = SingleTextView(options: singleTextViewOption)
        super.init(frame: .zero)
        setup()
    }
    
    required init?(coder aDecoder: NSCoder) {
        textView = SingleTextView(options: SingleTextViewOption())
        super.init(coder: aDecoder)
        setup()
    }
    
    private func setup() {
        self.addSubview(textView)

        // Configure Auto Layout for text view using SnapKit
        textView.snp.makeConstraints { make in
            make.leading.trailing.equalToSuperview()
            make.top.equalToSuperview()
            if textView.singleTextViewOption.enableAutoHeight {
                textViewHeightConstraint = make.height.equalTo(textView.intrinsicContentSize.height).constraint
            } else {
                make.height.equalTo(self.textView.singleTextViewOption.minHeight)
            }
        }
        
        if textView.singleTextViewOption.enableAutoHeight {
            textView.addObserver(self, forKeyPath: "contentSize", options: .new, context: nil)
        }
        
        self.backgroundColor = .clear
      
        
    }
    
    override func observeValue(forKeyPath keyPath: String?, of object: Any?, change: [NSKeyValueChangeKey : Any]?, context: UnsafeMutableRawPointer?) {
           if keyPath == "contentSize" {
               updateHeight()
           }
       }
    
    private func updateHeight() {
        // 텍스트 뷰의 콘텐츠 높이를 계산합니다.
        let contentHeight = textView.intrinsicContentSize.height
        
        // 높이 제약 조건을 업데이트합니다.
        textViewHeightConstraint?.update(offset: contentHeight)
        
        // 레이아웃을 강제로 갱신하여 변경 사항을 반영합니다.
        self.layoutIfNeeded()
        
        // 텍스트 뷰의 높이가 변경된 후에 실행될 필요가 있는 추가 작업을 수행합니다.
        self.textView.singleTextViewOption.onChangeHeight(contentHeight)
    }
    
    func setText(text: String) {
        textView.text = text
        self.textView.singleTextViewOption.onTextChanged(text)
    }
    
    deinit {
        print("TextView deinit")
        if textView.singleTextViewOption.enableAutoHeight {
            textView.removeObserver(self, forKeyPath: "contentSize")
        }
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
    }
    
    func activate () {
        self.textView.activate()
    }
    
    func deactivate () {
        self.textView.deactivate()
    }
}
