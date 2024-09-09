//
//  FontManager.swift
//  HypyG
//
//  Created by romi on 1/5/24.
//

import Foundation
import SwiftUI

///
/// Font extension
///
extension Font {
    enum SpoqaHanSansNeo {
        case bold
        case light
        case medium
        case regular
        case thin
        case custom(String)
        
        var value: String {
            switch self {
            case .bold:
                return "SpoqaHanSansNeo-Bold"
            case .light:
                return "SpoqaHanSansNeo-Light"
            case .medium:
                return "SpoqaHanSansNeo-Medium"
            case .regular:
                return "SpoqaHanSansNeo-Regular"
            case .thin:
                return "SpoqaHanSansNeo-Thin"
            case .custom(let name):
                return name
            }
        }
    }

    ///
    /// 커스텀 폰트를 불러다 쓰기 위함
    ///
    ///  사용 방법 :
    ///  
    /// ```swift
    /// // 기본
    /// Text("Hello, World!").font(.spoqa())
    /// // 지정하기
    /// Text("Hello, World!").font(.spoqa(.bold, size: 20))
    /// ```
    ///
    static func spoqa(_ type: SpoqaHanSansNeo = SpoqaHanSansNeo.regular, size: CGFloat = 15) -> Font {
        return .custom(type.value, size: size)
    }
}
