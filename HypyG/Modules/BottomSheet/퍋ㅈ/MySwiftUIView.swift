//
//  MySwiftUIView.swift
//  HypyG
//
//  Created by 온석태 on 9/9/24.
//

import Foundation
import SwiftUI

struct MySwiftUIView: View {
    @ObservedObject var vm:MySwiftUIViewModel = MySwiftUIViewModel()
    var pk:UUID
    
    @State var toggle: Bool = false
    @State var lastHeight: CGFloat = .zero
    @State var nowHeight: CGFloat = .zero
    @State var text: String = ""
    @State var text2: String = ""
    @State var text3: String = ""
    init(pk: UUID) {
        self.pk = pk
    }
    var body: some View {
        ZStack {
            VStack {
                Text("Hello, this is a SwiftUI View inside a UIViewController!")
                    .font(.title)
                    .padding()
                
                TextField("asdad", text: $text2)
                
                Button(action: {
                    nowHeight += 100
                    print("높이 토글")
                }) {
                    Text("Press Me")
                        .font(.headline)
                        .padding()
                        .background(Color.blue)
                        .foregroundColor(.white)
                        .cornerRadius(10)
                }
                
                TextField("asdad", text: $text)
                
                Button(action: {
//                    CustomBottomSheetSingleTone.shared.hide(pk: pk)
                    self.vm.keyboardManager.hideKeyboard()
                }) {
                    Text("닫기 버튼")
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
                    .frame(width: 20, height: nowHeight)
                
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
                
                TextField("asdad", text: $text3)
            }
        }
        .background(
            GeometryReader { geo -> Color in
                DispatchQueue.main.async {
                    print("size: \(geo.size.height)")
                    if lastHeight != geo.size.height {
                        
                        self.lastHeight = geo.size.height
                        CustomBottomSheetSingleTone.shared.updateSheetHeight(pk: pk, height: geo.size.height)
                    }
                    
                }
                return Color.clear
            }
        )
//        .ignoresSafeArea(.all)
//        .ignoresSafeArea(.keyboard)
       
        
    }
}


class MySwiftUIViewModel: ObservableObject {
    var bottomSheetOption: CustomUIKitBottomSheetOption?
    var navigationController: NavigationController? // 네비게이션
    var customUIKitBottomSheet: CustomUIKitBottomSheet?
    var keyboardManager: KeyboardManager = KeyboardManager()
    init() {
       
    }
    
    
    func setNavigationController(navigationController: NavigationController) {
        self.navigationController = navigationController
    }
    
    func openSheet() {
        let pk = UUID()
        let bottomSheetModel = CustomUIKitBottomSheetOption(pk: pk, someView: AnyView(MySwiftUIView(pk: pk)))
        self.bottomSheetOption = bottomSheetModel
        self.bottomSheetOption?.minimumHeight = 0
        self.bottomSheetOption?.backgroundColor = .blue.opacity(0.5)
        self.bottomSheetOption?.availableOutTouchClose = true
        self.bottomSheetOption?.sheetColor = .red
        self.bottomSheetOption?.handlerColor = .black
        self.bottomSheetOption?.dragAvailable = true
        self.bottomSheetOption?.sheetMode = .custom
        self.bottomSheetOption?.sheetHeight = 300
        
        if let bottomSheetModel = bottomSheetOption {
            let pk = bottomSheetModel.pk
            CustomBottomSheetSingleTone.shared.show(customUIKitBottomSheetOption: bottomSheetModel, viewPk: pk)
        }
    }
}
