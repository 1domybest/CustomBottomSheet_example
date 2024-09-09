//
//  SingleTextFieldContentView.swift
//  HypyG
//
//  Created by 온석태 on 8/30/24.
//

import Foundation
import UIKit

class SingleTextFieldContentView: UIView {
    
    private var textField: SingleTextField
    
    init(singleTextFieldOption: SingleTextFieldOption) {
        textField = SingleTextField(options: singleTextFieldOption)
        super.init(frame: .zero)
        setup()
    }
    
    required init?(coder aDecoder: NSCoder) {
        textField = SingleTextField(options: SingleTextFieldOption())
        super.init(coder: aDecoder)
        setup()
    }
    
    deinit {
        print("TextField deinit")
    }
    
    private func setup() {
        // Add the text field to the view
        self.addSubview(textField)
        
        // Configure Auto Layout for text field using SnapKit
        textField.snp.makeConstraints { make in
            make.leading.equalToSuperview()
            make.trailing.equalToSuperview()
            make.centerY.equalToSuperview()
//            make.center.equalToSuperview()
        }
        self.backgroundColor = .clear
    }
    
    func setText(text:String) {
        self.textField.setText(text: text)
    }
    
    func activate () {
        self.textField.activate()
    }
    
    func deactivate () {
        self.textField.deactivate()
    }
}
