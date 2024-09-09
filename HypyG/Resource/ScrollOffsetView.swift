//
//  ScrollOffsetView.swift
//  HypyG
//
//  Created by 윤예진 on 1/12/24.
//

import SwiftUI

///
/// coordinateSpace을 지정한 스크롤뷰의 스크롤 오프셋을 업데이트
///
/// - Parameters:
///    - coordinateSpace ( String ) : 스크롤뷰의 coordinateSpace의 키 네임 필요
///    - offset ( (CGFloat) -> () ) : 수직 offset 값을 전달하고 로직을 추가 가능
/// - Returns: View
///
extension View {
    @ViewBuilder
    func offsetView(coordinateSpace: String, offset: @escaping (CGFloat) ->()) -> some View {
        self.background(
            GeometryReader { proxy in
                let minY = proxy.frame(in: .named(coordinateSpace)).minY
                
                Color.clear
                    .preference(key: VerticalOffsetKey.self, value: minY)
                    .onPreferenceChange(VerticalOffsetKey.self) { value in
                        offset(value)
                    }
            }
        )
    }
}

struct VerticalOffsetKey: PreferenceKey {
    static var defaultValue: CGFloat = .zero
    static func reduce(value: inout CGFloat, nextValue: () -> CGFloat) {
        value += nextValue()
    }
}

struct HorizontalOffsetKey: PreferenceKey {
    static var defaultValue: CGFloat = .zero
    static func reduce(value: inout CGFloat, nextValue: () -> CGFloat) {
        value = nextValue()
    }
}
