//
//  NavigationController.swift
//  SwiftUIWithUIKit
//
//  Created by 온석태 on 2023/10/19.
//

import SwiftUI
import UIKit

///
/// 네비게이션 컨트롤러
///
/// - Parameters:
/// - Returns:
///
final class NavigationController: NSObject {

    public let window: UIWindow
    var rootViewController:CustomNavigationController?
    
    init(window: UIWindow) {
        self.window = window
    }
    
    ///
    /// 앱 첫시작시 실행되는 함수
    func showRootView() {
        setRootView(ContentView(), animated: true, viewName: .ContentView)
    }
    
    ///
    /// 네비게이션 이벤트 콜백 등록
    /// - Parameters:
    ///    - callback ( NavigationCallbackProtocol ) : 콜백
    ///    - viewPk: View 고유번호
    /// - Returns:
    func setCallback (callback: NavigationCallbackProtocol, viewPk: UUID) {
        self.rootViewController?.setCallback(callback: callback, viewPk: viewPk)
    }
    
    ///
    /// 최상단 부모 viewController 지정함수
    ///
    /// - Parameters:
    ///    - view ( T ) : SwiftUI View
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///
    func setRootView<T: View>(_ view: T, animated _: Bool, viewName: ViewName, contentId: String? = nil) {
        if self.rootViewController != nil {
            self.rootViewController?.deinitialize()
        }
        
        
        let viewPk = UUID()
        let view = view
            .environment(\.navigationController, self)
            .environment(\.viewPK, viewPk)
            .environment(\.contentId, contentId)
            .navigationBarHidden(true)
            .ignoresSafeArea(.all)
        
        let hostingView = CustomHostingController(rootView: view)
        hostingView.navigationItem.hidesBackButton = true
        
        hostingView.viewName = viewName
        hostingView.viewPk = viewPk
        hostingView.contentId = contentId
        
        
        let customNavController = CustomNavigationController(rootViewController: hostingView)
        
        
        customNavController.interactivePopGestureRecognizer?.delegate = self
        customNavController.delegate = customNavController

        self.rootViewController = customNavController
        
        window.rootViewController = customNavController
        
    }
  
    ///
    /// SwiftUI 의 중복뷰의 스택을 방지해주는 함수
    ///
    /// - Parameters:
    ///    - view ( ``ViewName`` ) : View 고유이름
    ///    - contentId : 각 뷰가 가지고있는 고유 번호 (예) 서버에서 받아오는 pk Id
    /// - Returns:
    ///
    func validateDoubleNavigation (viewName: ViewName, contentId: String? = nil) -> Bool {
        guard let contentId = contentId else { return true }
        
        if let previousViewController = self.getPreviousViewController() {
            let beforeContentId = previousViewController.contentId
            let beforeViewName = previousViewController.viewName
            let viewName = viewName

            if beforeViewName == viewName && beforeContentId == contentId {
                return false
            }

        }
        
        return true
    }
    
    ///
    /// 이전 UIViewController를 받아오는 함수
    func getPreviousViewController() -> UIViewController? {
        if let navigationController = self.rootViewController {
            let viewControllers = navigationController.viewControllers
            if viewControllers.count > 1 {
                return viewControllers[viewControllers.count - 2]
            }
        }
        return nil
    }
    
    ///
    /// 현재 UIViewController를 받아오는 함수
    func getCurrentViewController() -> UIViewController? {
        if let topViewController = self.rootViewController?.topViewController {
            return topViewController
        }

        return nil
    }


}

extension NavigationController: UIGestureRecognizerDelegate {
    public func gestureRecognizerShouldBegin(_ gestureRecognizer: UIGestureRecognizer) -> Bool {
        if let panGestureRecognizer = gestureRecognizer as? UIPanGestureRecognizer, let view = panGestureRecognizer.view {
            let velocity = panGestureRecognizer.velocity(in: view)
            let isHorizontalSwipe = abs(velocity.x) > abs(velocity.y)
            
            if isHorizontalSwipe {
                let isSwipeEnable = self.getCurrentViewController()?.isSwipeEnable ?? false
                return isSwipeEnable
            } else {
                return false
            }
        }
        return false
    }
}


