//
//  Color.swift
//  UIKitCustomBottomSheet
//
//  Created by 온석태 on 12/17/24.
//

import Foundation
import UIKit
import SwiftUI

extension Color {
    ///
    /// 원하는 컬러 생성
    ///
    ///  사용 방법 :
    ///
    /// ```
    /// Text("안녕하세요, SwiftUI!")
    ///   .font(.title)
    ///   .foregroundColor(Color.gray6)
    /// ```
    ///
    static let black0 = "#000000".hexToColor()! //hex: "#000000"
    static let pink0 = "#FFB2EC".hexToColor()! //hex: "#FFB2EC"
    static let pink1 = "#E52CB6".hexToColor()! //hex: "#E52CB6"
    static let green0 = "#AEE652".hexToColor()! //hex: "#8EDB11"
    static let green1 = "#8EDB11".hexToColor()! //hex: "#8EDB11"
    static let blue0 = "#92B0FF".hexToColor()! //hex: "#4A7DFF"
    static let blue1 = "#4A7DFF".hexToColor()! //hex: "#4A7DFF"
    static let skyblue0 = "#B8BFFF".hexToColor()! //hex: "#B8BFFF"
    static let mint0 = "#85E9E3".hexToColor()! //hex: "#46D6CD"
    static let mint1 = "#46D6CD".hexToColor()! //hex: "#46D6CD"
    static let orange0 = "#FFC9A1".hexToColor()! //hex: "#FF964A"
    static let orange1 = "#FF964A".hexToColor()! //hex: "#FF964A"
    static let red0 = Color("Red") //hex: "#FF0000"
    static let white0 = Color("White") //hex: "#FFFFFF"
    static let blueViolet0 = Color("BlueViolet0") //hex: "#A686FF"
    static let blueViolet1 = Color("BlueViolet1") //hex: "#794AFF"
       
    // 지정완료
    static let gray0 = Color("Gray0") //hex: "#f8f9fa"
    static let gray1 = Color("Gray1") //hex: "#f1f3f5"
    static let gray2 = Color("Gray2") //hex: "#e9ecef"
    static let gray3 = Color("Gray3") //hex: "#dee2e6"
    static let gray4 = Color("Gray4") //hex: "#ced4da"
    static let gray5 = Color("Gray5") //hex: "#adb5bd"
    static let gray6 = Color("Gray6") //hex: "#868e96"
    static let gray7 = Color("Gray7") //hex: "#495057"
    static let gray8 = Color("Gray8") //hex: "#343a40"
    static let gray9 = Color("Gray9") //hex: "#212529"
    
    func getUIColor () -> UIColor {
        return UIColor(self)
    }
}
