//
//  Environment+NavigationController.swift
//  HypyG
//
//  Created by 윤예진 on 12/6/23.
//

import Foundation
import SwiftUI

/**
 환경 변수 + 네비게이션
 */

private struct NavigationControllerKey: EnvironmentKey {
    static let defaultValue: NavigationController? = nil
}

private struct CustomUIKitBottomSheetKey: EnvironmentKey {
    static let defaultValue: CustomUIKitBottomSheet? = nil
}

extension EnvironmentValues {
    var navigationController: NavigationController? {
        get { self[NavigationControllerKey.self] }
        set { self[NavigationControllerKey.self] = newValue }
    }
    
    var customUIKitBottomSheet: CustomUIKitBottomSheet? {
        get { self[CustomUIKitBottomSheetKey.self] }
        set { self[CustomUIKitBottomSheetKey.self] = newValue }
    }
}

extension View {
    func envNavigation(_ navigationController: NavigationController?) -> some View {
        environment(\.navigationController, navigationController)
    }
}
