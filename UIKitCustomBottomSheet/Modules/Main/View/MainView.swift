//
//  MainView.swift
//  UIKitCustomBottomSheet
//
//  Created by 온석태 on 12/17/24.
//

import Foundation
import SwiftUI

struct MainView: View {
    @ObservedObject var vm: MainViewModel = MainViewModel()
    var body: some View {
        ZStack {
            Color.black
            
            Button(action: {
                self.vm.openSwiftUIBottomSheet()
            }, label: {
                Text("바텀시트 열기")
                    .padding(.horizontal, 20)
                    .padding(.vertical, 15)
                    .foregroundColor(.white)
                    .background(
                        RoundedRectangle(cornerRadius: 10)
                            .foregroundColor(.blue)
                    )
            })
            
        }
    }
}
