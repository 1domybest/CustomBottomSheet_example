//
//  MySwiftUIView.swift
//  HypyG
//
//  Created by 온석태 on 9/9/24.
//

import Foundation
import SwiftUI
import CustomBottomSheetLibrary

struct CustomBottomSheetView: View {
    @ObservedObject var vm:CustomBottomSheetViewModel

    init(pk: UUID) {
        vm = CustomBottomSheetViewModel(pk: pk)
    }
    var body: some View {
        ZStack {
            VStack {
                Text("Hello, this is a SwiftUI View inside a UIViewController!")
                    .font(.title)
                    .foregroundColor(.red)
                    .padding()
                
                UIKitViewRepresentable(view: self.vm.textViewView)
                    .frame(height: self.vm.textViewHeight)
                
                Button(action: {
                    vm.extraHeight += 100
                    print("높이 토글")
                }) {
                    Text("시트 높이 늘리기")
                        .font(.headline)
                        .padding()
                        .background(Color.blue)
                        .foregroundColor(.white)
                        .cornerRadius(10)
                }
                
                UIKitViewRepresentable(view: self.vm.textFieldView)
                    .frame(height: 50)
                
                Button(action: {
                    CustomBottomSheetSingleTone.shared.hide(pk: self.vm.pk)
                }) {
                    Text("시트 닫기 버튼")
                        .font(.headline)
                        .padding()
                        .background(Color.blue)
                        .foregroundColor(.white)
                        .cornerRadius(10)
                }
                
                Button(action: {
                    self.vm.keyboardManager?.hideKeyboard()
                }) {
                    Text("키보드닫기 버튼")
                        .font(.headline)
                        .padding()
                        .background(Color.blue)
                        .foregroundColor(.white)
                        .cornerRadius(10)
                }
                
                
                Button(action: {
                    self.vm.openSheet()
                }) {
                    Text("추가 시트 버튼")
                        .font(.headline)
                        .padding()
                        .background(Color.blue)
                        .foregroundColor(.white)
                        .cornerRadius(10)
                }
                
                RoundedRectangle(cornerRadius: 20)
                    .frame(width: 20, height: vm.extraHeight)
                
                Button(action: {
                    CustomBottomSheetSingleTone.shared.hideAll()
                }) {
                    Text("모든 시트 닫기")
                        .font(.headline)
                        .padding()
                        .background(Color.blue)
                        .foregroundColor(.white)
                        .cornerRadius(10)
                }
            }
        }
        .background(
            GeometryReader { geo -> Color in
                DispatchQueue.main.async {
                    print("size: \(geo.size.height)")
                    if vm.hasChild { return }
                    if vm.lastHeight != geo.size.height {
                        self.vm.lastHeight = geo.size.height
                        CustomBottomSheetSingleTone.shared.updateSheetHeight(pk: self.vm.pk, height: geo.size.height)
                    }
                    
                }
                return Color.clear
            }
        )
        .ignoresSafeArea(.all)
        .ignoresSafeArea(.keyboard)
    }
}



