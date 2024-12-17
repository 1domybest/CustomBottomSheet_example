//
//  String.swift
//  UIKitCustomBottomSheet
//
//  Created by 온석태 on 12/17/24.
//

import Foundation
import SwiftUI

extension String {
    func hexToColor() -> Color? {
        // # 문자 제거
        var hexSanitized = self.trimmingCharacters(in: .whitespacesAndNewlines)
        hexSanitized = hexSanitized.replacingOccurrences(of: "#", with: "")

        var rgb: UInt64 = 0
        Scanner(string: hexSanitized).scanHexInt64(&rgb)
        
        var red: Double = 0
        var green: Double = 0
        var blue: Double = 0
        var opacity: Double = 1.0
        if hexSanitized.count == 6 {
            red = Double((rgb & 0xFF0000) >> 16) / 255.0
            green = Double((rgb & 0x00FF00) >> 8) / 255.0
            blue = Double(rgb & 0x0000FF) / 255.0
        } else {
            red = Double((rgb >> 24) & 0xFF) / 255.0
            green = Double((rgb >> 16) & 0xFF) / 255.0
            blue = Double((rgb >> 8) & 0xFF) / 255.0
            opacity = Double(rgb & 0xFF) / 255.0
        }
        return Color(red: red, green: green, blue: blue, opacity: opacity)
    }
}
