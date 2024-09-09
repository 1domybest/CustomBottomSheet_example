//
//  ContentView.swift
//  SwiftUIWithUIKit
//
//  Created by 온석태 on 2023/10/19.
//

import SwiftUI
import AVFoundation

struct ContentView: View {
    @UIApplicationDelegateAdaptor(AppDelegate.self) var delegate // register app delegate for Firebase setup
    
    @Environment(\.navigationController) var navigationController: NavigationController? // 네비게이션
    
    @ObservedObject var vm: ContentViewModel = ContentViewModel()
    
    init() {
    }
    
    var body: some View {
        ZStack {
            Button(action: {
                self.vm.openSheet()
            }, label: {
                Text("안녕")
            })
        }
        .onAppear {
            self.vm.setNavigationController(navigationController: self.navigationController!)
        }
    }
}



class ContentViewModel: ObservableObject {
    var bottomSheetOption: CustomUIKitBottomSheetOption?
    var navigationController: NavigationController? // 네비게이션
    var customUIKitBottomSheet: CustomUIKitBottomSheet?
    init() {
        let pk = UUID()
        let bottomSheetModel = CustomUIKitBottomSheetOption(pk: pk, someView: AnyView(MySwiftUIView(pk: pk)))
        self.bottomSheetOption = bottomSheetModel
        self.bottomSheetOption?.minimumHeight = 0
        self.bottomSheetOption?.backgroundColor = .blue.opacity(0.5)
        self.bottomSheetOption?.availableOutTouchClose = true
        self.bottomSheetOption?.hasKeyboard = true
        self.bottomSheetOption?.sheetColor = .red
        self.bottomSheetOption?.handlerColor = .black
        self.bottomSheetOption?.dragAvailable = true
        self.bottomSheetOption?.sheetMode = .custom
        self.bottomSheetOption?.sheetHeight = 450
    }
    
    
    func setNavigationController(navigationController: NavigationController) {
        self.navigationController = navigationController
    }
    
    func openSheet() {
        if let bottomSheetModel = bottomSheetOption {
            let pk = bottomSheetModel.pk
            CustomBottomSheetSingleTone.shared.show(customUIKitBottomSheetOption: bottomSheetModel, viewPk: pk)
        }
    }
}
