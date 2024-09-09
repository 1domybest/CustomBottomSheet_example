//
//  CustomBottomSheetSingleTone.swift
//  HypyG
//
//  Created by 온석태 on 9/9/24.
//

import Foundation
import UIKit

class CustomBottomSheetSingleTone {
    /// 싱글톤
    public static var shared = CustomBottomSheetSingleTone()
    
    var bottomSheetList: [(CustomUIKitBottomSheet, UUID)] = []
    var topViewController:UIViewController?
    func updateSheetHeight(pk: UUID, height:CGFloat) {
        // 필터링을 위해 for-in 루프와 enumerated()를 사용하여 인덱스와 요소를 동시에 가져옴
        for (index, (bottomSheetViewController, bottomSheetUUID)) in bottomSheetList.enumerated().reversed() {
            if bottomSheetUUID == pk {
                bottomSheetViewController.updateSheetHeight(newHeight: height)
            }
        }
    }
    
    func hide(pk: UUID?) {
        guard let viewPk = pk else { return }
        // 필터링을 위해 for-in 루프와 enumerated()를 사용하여 인덱스와 요소를 동시에 가져옴
        for (index, (bottomSheetViewController, bottomSheetUUID)) in bottomSheetList.enumerated().reversed() {
            if bottomSheetUUID == viewPk {
                // Alert를 dismiss 처리
                bottomSheetViewController.dismissPresent()
                bottomSheetViewController.unreference()
                // 배열에서 해당 요소를 제거
                bottomSheetList.remove(at: index)
            }
        }
        
        // 배열이 비어 있으면 nil로 설정
        if bottomSheetList.isEmpty {
            bottomSheetList = []
        }
    }
    
    func hideAll() {
        DispatchQueue.main.async {
             self.dismissViewControllersRecursively()
         }
    }

    private func dismissViewControllersRecursively() {
        guard !bottomSheetList.isEmpty else {
            guard let topViewController = self.topViewController else { return }
            topViewController.dismiss(animated: true)
            self.topViewController = nil
            return
        }
        
        let (viewControllerToDismiss, pk) = bottomSheetList.popLast()!
        
        DispatchQueue.global(qos: .background).async { [weak self] in
            guard let self = self else { return }
            // 백그라운드에서 unreference 호출
            viewControllerToDismiss.unreference()
            
            // 다시 메인 스레드로 돌아와서 재귀 호출
            DispatchQueue.main.async {
                self.dismissViewControllersRecursively()
            }
        }
    }
    
    func show(customUIKitBottomSheetOption: CustomUIKitBottomSheetOption, viewPk: UUID) {
        let bottomSheetViewController = CustomUIKitBottomSheet(bottomSheetModel: customUIKitBottomSheetOption)
        
        guard let topController = getTopViewController() else { return }
        if self.bottomSheetList.isEmpty { self.topViewController = topController }
        
        topController.present(bottomSheetViewController, animated: true)
        self.bottomSheetList.append((bottomSheetViewController, viewPk))
    }
    
    private func getTopViewController() -> UIViewController? {
        guard let rootViewController = UIApplication.shared.connectedScenes
                .compactMap({ $0 as? UIWindowScene })
                .first?.windows
                .filter({ $0.isKeyWindow }).first?.rootViewController else {
            
            DispatchQueue.main.asyncAfter(deadline: .now() + 1) {
                     _ = self.getTopViewController()
                 }
            
            return nil
        }
        
        return getTopViewController(from: rootViewController)
    }
    
    private func getTopViewController(from rootViewController: UIViewController) -> UIViewController? {
        if let presentedViewController = rootViewController.presentedViewController {
            // rootViewController가 다른 뷰 컨트롤러를 표시 중이면, 그 뷰 컨트롤러를 최상단으로 확인
            return getTopViewController(from: presentedViewController)
        }
        
        // Navigation Controller가 있는 경우
        if let navigationController = rootViewController as? UINavigationController {
            return navigationController.visibleViewController
        }
        
        // Tab Bar Controller가 있는 경우
        if let tabBarController = rootViewController as? UITabBarController {
            if let selectedViewController = tabBarController.selectedViewController {
                return getTopViewController(from: selectedViewController)
            }
        }
        
        return rootViewController
    }
}
