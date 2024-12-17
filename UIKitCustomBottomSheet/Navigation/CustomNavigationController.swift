//
//  CustomNavigationController.swift
//  UIkit+SwiftUI
//
//  Created by 온석태 on 10/3/24.
//

import Foundation
import UIKit


class CustomNavigationController: UINavigationController, UINavigationControllerDelegate {
    var shouldPushViewController: ((UIViewController) -> Bool)?
    var callbackList: [(NavigationCallbackProtocol, UUID)] = []

    
    deinit {
        print("CustomNavigationController Deinit")
    }
    
    /// 기존 콜백으로 등록된 리스트 제거후 콜백 호출
    func deinitialize() {
        for viewController in self.viewControllers {
            let viewName = viewController.viewName ?? .None
            let viewPk = viewController.viewPk ?? UUID()
            let isSwipeEnable = viewController.isSwipeEnable ?? false
            
            for (index, (callback, pk)) in self.callbackList.enumerated().reversed() {
                if viewPk == pk {
                    self.callbackList.remove(at: index)
                    callback.didPop(viewName: viewName, isSwipe: isSwipeEnable, viewPk: viewPk)
                }
            }
            
        }
        
        self.callbackList.removeAll()
    }
    
    /// 콜백 등록
    func setCallback(callback: NavigationCallbackProtocol, viewPk: UUID) {
        if !callbackList.contains(where: { $0.1 == viewPk }) {
            callbackList.append((callback, viewPk))
            print("콜백 추가됨, 갯수 \(callbackList.count)")
        } else {
            print("콜백 이미 존재하는 UUID입니다. 추가하지 않음.")
        }

        print("콜백 내용 \(callbackList)")
    }

    /// 화면의 가로세로 모드를 정방향으로 고정 [필요에따라 변경]
    override var supportedInterfaceOrientations: UIInterfaceOrientationMask {
        return topViewController?.supportedInterfaceOrientations ?? .portrait
    }
    /// supportedInterfaceOrientations 와 마찬가지로 가장 최상단뷰의 방향모드를 따라감 고로 이코드에서는
    /// 방향전환을 허용하지않음
    override var shouldAutorotate: Bool {
        topViewController?.shouldAutorotate ?? true
    }
    
    /// push한 컨트롤러 (혹은 HostingController) 가 렌더링됬을때의 이벤트
    func navigationController(_ navigationController: UINavigationController, didShow viewController: UIViewController, animated: Bool) {
        if let fromViewController = transitionCoordinator?.viewController(forKey: .from),
           !navigationController.viewControllers.contains(fromViewController) {
            let isSwipeEnable = fromViewController.isSwipeEnable ?? false

            // 실제로 pop이 완료된 상태일 때만 호출
            let viewName = fromViewController.viewName ?? .None
            let viewPk = fromViewController.viewPk ?? UUID()

            for (index, (callback, pk)) in self.callbackList.enumerated().reversed() {
                if viewPk == pk {
                    self.callbackList.remove(at: index)
                    callback.didPop(viewName: viewName, isSwipe: isSwipeEnable, viewPk: viewPk)
                }
            }
        }
    }

    /// UINavigationController에 있는 내장 함수로 오버라이드를 사용하여 한번에 여러가지 컨트롤러를 push했을때
    /// 커스텀 로직을 추가할수있다
    override func setViewControllers(_ viewControllers: [UIViewController], animated: Bool) {
        var viewControllers = viewControllers
        let replacedControllerView = viewControllers.removeLast()

        let viewName = replacedControllerView.viewName ?? .None
        let viewPk = replacedControllerView.viewPk ?? UUID()
        let isSwipe = replacedControllerView.isSwipeEnable ?? false

        for (index, (callback, pk)) in self.callbackList.enumerated().reversed() {
            if viewPk == pk {
                self.callbackList.remove(at: index)
                callback.didPop(viewName: viewName, isSwipe: isSwipe, viewPk: viewPk)
            }
        }
        super.setViewControllers(viewControllers, animated: animated)
    }

    /// UINavigationController에 있는 내장 함수로 오버라이드를 사용하면 push를 했을시
    /// 커스텀 로직을 추가할수있다
    override func popViewController(animated: Bool) -> UIViewController? {
        let poppedViewController = super.popViewController(animated: animated)
        let isSwipeEnable = poppedViewController?.isSwipeEnable ?? false

        if !isSwipeEnable {
            let viewName = poppedViewController?.viewName ?? .None
            let viewPk = poppedViewController?.viewPk ?? UUID()
            let isSwipe = poppedViewController?.isSwipeEnable ?? false

            for (index, (callback, pk)) in self.callbackList.enumerated().reversed() {
                if viewPk == pk {
                    self.callbackList.remove(at: index)
                    callback.didPop(viewName: viewName, isSwipe: isSwipe, viewPk: viewPk)
                }
            }
        }


        return poppedViewController
    }

    /// UINavigationController에 있는 내장 함수로 오버라이드를 사용하면 pop를 했을시
    /// 커스텀 로직을 추가할수있다
    override func pushViewController(_ viewController: UIViewController, animated: Bool) {
         if let shouldPush = shouldPushViewController {
             if !shouldPush(viewController) {
                 return
             }
         } else if !viewController.shouldPush() {
             return
         }

         if let topViewController = self.topViewController,
            let topViewName = topViewController.viewName,
            let newViewName = viewController.viewName,
            topViewName == newViewName {
             if DoublePushViewName(rawValue: newViewName.rawValue) != nil {
                 return
             }
         }

         super.pushViewController(viewController, animated: animated)
     }
}
