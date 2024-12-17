//
//  SwiftUINavigationController.swift
//  UIkit+SwiftUI
//
//  Created by 온석태 on 12/17/24.
//

import Foundation
import SwiftUI
import UIKit

/// SwiftUI
extension NavigationController {
    ///
    /// SwiftUI 의 View 를 변환후 직접 push 하는 함수
    ///
    /// - Parameters:
    ///    - view ( T ) : UIKitt SwiftUI View
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///
    func push<T: View>(_ view: T, animated: Bool, swipeActivation: Bool = true, viewName: ViewName, contentId: String? = nil) {
        if !self.validateDoubleNavigation(viewName: viewName, contentId: contentId) {
            self.pop()
            return
        }
        
        let viewPk = UUID()
        let view = view
            .environment(\.navigationController, self)
            .environment(\.viewPK, viewPk)
            .environment(\.contentId, contentId)
            .navigationBarHidden(true)
            .ignoresSafeArea(.all)
        
        let hostingView = CustomHostingController(rootView: view.environment(\.navigationController, self).navigationBarHidden(true).ignoresSafeArea(.all))
        hostingView.navigationItem.hidesBackButton = true
        
        hostingView.viewName = viewName
        hostingView.viewPk = viewPk
        hostingView.contentId = contentId
        hostingView.isSwipeEnable = swipeActivation

        if let navigationController = self.rootViewController {
            navigationController.setToolbarHidden(true, animated: true)
            navigationController.setNavigationBarHidden(true, animated: false)

            navigationController.pushViewController(hostingView, animated: animated)
        }
    }
    
    ///
    /// SwiftUI 의 View 를 변환후 직접 push 하는 함수 + 화면전환 방향 커스텀 지정
    ///
    /// - Parameters:
    ///    - view ( T ) : UIKitt SwiftUI View
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    ///    - type ( CATransitionSubtype ) : 화면전환 방향
    /// - Returns:
    ///
    func push<T: View>(_ view: T, animated _: Bool, type: CATransitionSubtype, animationType: CATransitionType = .moveIn, swipeActivation: Bool = true, viewName: ViewName, contentId: String? = nil) {
        
        if !self.validateDoubleNavigation(viewName: viewName, contentId: contentId) {
            self.pop()
            return
        }
        
        let viewPk = UUID()
        let view = view
            .environment(\.navigationController, self)
            .environment(\.viewPK, viewPk)
            .environment(\.contentId, contentId)
            .navigationBarHidden(true)
            .ignoresSafeArea(.all)
        
        let hostingView = UIHostingController(rootView: view)
        
        hostingView.navigationItem.hidesBackButton = true
        
        hostingView.viewName = viewName
        hostingView.viewPk = viewPk
        hostingView.contentId = contentId
        hostingView.isSwipeEnable = swipeActivation

        if let navigationController = self.rootViewController {
            navigationController.isNavigationBarHidden = true
            let transition = CATransition()
            transition.duration = 0.3
            transition
                .timingFunction = CAMediaTimingFunction(name: CAMediaTimingFunctionName.easeInEaseOut)
            transition.type = animationType
            transition.subtype = type
            navigationController.view.layer.add(transition, forKey: kCATransition)
                
            navigationController.pushViewController(hostingView, animated: false)
        }
    }
    
    ///
    /// 현재 렌더링된 뷰를 다른 뷰로 교체할때 사용
    func replaceTopView<T: View>(_ view: T, animated: Bool, swipeActivation: Bool = true, viewName: ViewName, contentId: String? = nil) {
        
        if !self.validateDoubleNavigation(viewName: viewName, contentId: contentId) {
            self.pop()
            return
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
        hostingView.isSwipeEnable = swipeActivation

        if let navigationController = self.rootViewController {
            
            var viewControllers = navigationController.viewControllers

            let replacedControllerView = viewControllers.removeLast()
            replacedControllerView.isReplaced = true

            viewControllers.append(hostingView)
            viewControllers.append(replacedControllerView)

            navigationController.setViewControllers(viewControllers, animated: animated)
        }
    }
    
    ///
    /// 현재 렌더링된 뷰를 다른 뷰로 교체할때 사용
    func replaceTopView<T: View>(_ view: T, type: CATransitionSubtype, animationType: CATransitionType = .moveIn, swipeActivation: Bool = true, viewName: ViewName, contentId: String? = nil) {
        
        if !self.validateDoubleNavigation(viewName: viewName, contentId: contentId) {
            self.pop()
            return
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
        hostingView.isReplaced = true
        hostingView.isSwipeEnable = swipeActivation

        if let navigationController = self.rootViewController {
            
            let transition = CATransition()
            transition.duration = 0.3
            transition.timingFunction = CAMediaTimingFunction(name: CAMediaTimingFunctionName.easeInEaseOut)
            transition.type = animationType
            transition.subtype = type
            navigationController.view.layer.add(transition, forKey: kCATransition)
            
            var viewControllers = navigationController.viewControllers
            
            let replacedControllerView = viewControllers.removeLast()
            replacedControllerView.isReplaced = true

            viewControllers.append(hostingView)
            viewControllers.append(replacedControllerView)

            navigationController.setViewControllers(viewControllers, animated: false)
        }
    }
}

/// UIKit
extension NavigationController {
    ///
    /// UIKit 의 ViewController 를 직접 push 하는 함수
    ///
    /// - Parameters:
    ///    - viewController ( UIViewController ) : UIKitt ViewController
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///
    func push(viewController: UIViewController, animated: Bool, swipeActivation: Bool = true, viewName: ViewName, contentId: String? = nil) {
        
        if !self.validateDoubleNavigation(viewName: viewName, contentId: contentId) {
            self.pop()
            return
        }
        
        if let navigationController = window.rootViewController as? CustomNavigationController {
            let viewPk = UUID()
            
            viewController.viewName = viewName
            viewController.viewPk = viewPk
            viewController.contentId = contentId
            viewController.isSwipeEnable = swipeActivation

            navigationController.setToolbarHidden(true, animated: true)
            navigationController.setNavigationBarHidden(true, animated: false)
            navigationController.pushViewController(viewController, animated: animated)
        }
    }
}
