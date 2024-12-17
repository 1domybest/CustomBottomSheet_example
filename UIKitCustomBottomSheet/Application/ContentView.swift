//
//  ContentView.swift
//  SwiftUIWithUIKit
//
//  Created by 온석태 on 2023/10/19.
//

import SwiftUI

struct ContentView: View {
    @UIApplicationDelegateAdaptor(AppDelegate.self) var delegate // register app delegate for Firebase setup
    
    @Environment(\.navigationController) var navigationController: NavigationController? // 네비게이션
    
    var body: some View {
        ZStack {
            
            VStack {
                Button(action: {
                    navigationController?.push(MainView(), animated: true, viewName: .MainView)
                }, label: {
                    Text("MainView 로 넘어가기")
                        .padding(.horizontal, 20)
                        .padding(.vertical, 15)
                        .foregroundColor(.white)
                        .background(
                            RoundedRectangle(cornerRadius: 10)
                                .foregroundColor(.red)
                        )
                })
                
            }
            
        }
    }
}
