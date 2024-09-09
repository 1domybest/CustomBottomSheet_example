///
/// 중복 푸시를 원하는 view 이름
///
/// - Parameters:
/// - Returns:
///
enum DoublePushViewName: String {
    case None
}

///
/// 푸시할 view 이름
///
/// - Parameters:
/// - Returns:
///
enum ViewName: String {
    // 테스트 페이지
    case TestIdolHomeView

    case ContentView
    case ProducerLiveStreamingEndView
    case DefaultAlertSheet
    case IdolLiveStreamingEndView
    case ProfileSetupView
    case ProfileHomeView
    case AuthSignInView
    case PhoneVerificationView
    case TermsOfServiceView
    case IdolExternalStreamingView
    case IdolHomeView
    case ProducerHomeView
    case MyVoteDetailView
    case StageDetailView
    case StorableTileView
    case MyTileBagView
    case DelegateBagView
    case ProfileEditView
    case UIActivityViewController
    case IdolLiveStreamingView
    case IdolLiveStreamingEntryView
    case VODView
    case ProjectHomeDetailView
    case RankingView
    case ProjectApplicationView
    case ProducerLiveStreamingView
    case ProducerLiveStreamingEntryView
    case LookAroundHomeView
    case JobChoiceView
    case TileView
    case DetailTileView
    case PreContractCodeView
    case SelectSkillTendencyView
    case TileCompleteView
    case IdolApplyView
    case IdolApplyWaitingView
    case CurrencyHomeView
    case ProductListView
    case SettingHomeView
    case EventNoticeView
    case LegalConsentView
    case NotificationSettingView
    case TermsDocumentView
    case BlockSettingView
    case BlockDetailView
    case IdolApplyRejectedView
    case WithdrawMemberView
    case FAQView
    
    case NonMemberProducerLiveStreamingEndView
    case NonMemberProducerLiveStreamingView
    case NonMemberVODView
    
    case None
}

//
//  NavigationController.swift
//  SwiftUIWithUIKit
//
//  Created by 온석태 on 2023/10/19.
//

import SwiftUI
import UIKit
private var AssociatedObjectHandle: UInt8 = 0
///
/// 네비게이션 컨트롤러
///
/// - Parameters:
/// - Returns:
///
final class NavigationController: NSObject {

    
    
    private let window: UIWindow
    public var presentController:CustomTransitionDelegate?
    var swipeScreenOrderList: [Bool] = []
    var swipeEnable:Bool = true
    
    init(window: UIWindow) {
        self.window = window
    }
    
    ///
    /// 앱 첫시작시 실행되는 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func showRootView() {
        setRootView(ContentView(), animated: true, viewName: .ContentView)
    }
    
    
    ///
    /// 최상단 부모 viewController 지정함수
    ///
    /// - Parameters:
    ///    - view ( T ) : SwiftUI View
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///
    func setRootView<T: View>(_ view: T, animated _: Bool, isLandscape: Bool = false, viewName: ViewName) {
        let swiftUIView = view.navigationBarHidden(true).environment(\.navigationController, self)
        let hostingView = CustomHostingController(rootView: swiftUIView, isLandscape: isLandscape)
        hostingView.navigationController?.isNavigationBarHidden = true
        hostingView.viewName = viewName
        
        let customNavController = CustomNavigationController(rootViewController: hostingView)
        window.rootViewController = customNavController
        
        customNavController.interactivePopGestureRecognizer?.delegate = self
        customNavController.delegate = self
    }
    
    
    ///
    /// 여러 Screen 을 한번에 push 할때 사용하는 함수
    ///
    /// - Parameters:
    ///    - views ( [any View] ) : SwiftUI View 리스트
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///
    func pushViewListPush(_ views: [any View], animated _: Bool, viewStack: [ViewName]) {
        for (index, view) in views.enumerated() {
            view.environment(\.navigationController, self)
            if index == 0 {
                // 첫 번째 뷰일 경우 루트 뷰로 설정
                setRootView(view, animated: false, viewName: viewStack[index])
            } else {
                // 그 외의 경우 푸시
                push(view, animated: false, viewName: viewStack[index])
            }
        }
    }
    
    ///
    /// UIKit 의 ViewController 를 직접 push 하는 함수
    ///
    /// - Parameters:
    ///    - viewController ( UIViewController ) : UIKitt ViewController
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///
    func push(viewController: UIViewController, animated: Bool, swipeActivation: Bool = true, viewName: ViewName) {
        if let navigationController = window.rootViewController as? CustomNavigationController {
            viewController.viewName = viewName
            self.swipeScreenOrderList.append(swipeActivation)
            navigationController.setToolbarHidden(true, animated: true)
            navigationController.setNavigationBarHidden(true, animated: false)
            navigationController.pushViewController(viewController, animated: animated)
        }
    }

    
    ///
    /// SwiftUI 의 View 를 변환후 직접 push 하는 함수
    ///
    /// - Parameters:
    ///    - view ( T ) : UIKitt SwiftUI View
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///
    func push<T: View>(_ view: T, animated: Bool, swipeActivation: Bool = true, isLandscape: Bool = false, viewName: ViewName) {
        let hostingView = CustomHostingController(rootView: view.environment(\.navigationController, self).navigationBarHidden(true).ignoresSafeArea(.all), isLandscape: isLandscape)
        hostingView.navigationItem.hidesBackButton = true
        hostingView.viewName = viewName
        // 'window.rootViewController'가 'CustomNavigationController'의 인스턴스인지 확인
        if let navigationController = window.rootViewController as? CustomNavigationController {
            self.swipeScreenOrderList.append(swipeActivation)
            navigationController.setToolbarHidden(true, animated: true)
            navigationController.setNavigationBarHidden(true, animated: false)

            navigationController.pushViewController(hostingView, animated: animated)
        }
    }
    
    func replaceTopView<T: View>(_ view: T, animated: Bool, swipeActivation: Bool = true, isLandscape: Bool = false, viewName: ViewName) {
        let hostingView = CustomHostingController(rootView: view.environment(\.navigationController, self).navigationBarHidden(true).ignoresSafeArea(.all), isLandscape: isLandscape)
        hostingView.navigationItem.hidesBackButton = true
        hostingView.viewName = viewName
        if let navigationController = window.rootViewController as? CustomNavigationController {
            
            swipeScreenOrderList.append(swipeActivation)
            
            // 현재 네비게이션 스택을 가져옵니다.
            var viewControllers = navigationController.viewControllers
            
            // 스택의 마지막 뷰 컨트롤러를 제거합니다.
            if !viewControllers.isEmpty {
                let lastOne = viewControllers.removeLast()
            }

            
            // 새로운 호스팅 뷰 컨트롤러를 스택에 추가합니다.
            viewControllers.append(hostingView)
            // 수정된 스택을 네비게이션 컨트롤러에 설정합니다.
            navigationController.setViewControllers(viewControllers, animated: animated)
        }
    }
    
    func replaceTopView<T: View>(_ view: T, type: CATransitionSubtype, animationType: CATransitionType = .moveIn, swipeActivation: Bool = true, isLandscape: Bool = false, viewName: ViewName) {
        let hostingView = CustomHostingController(rootView: view.environment(\.navigationController, self).navigationBarHidden(true).ignoresSafeArea(.all), isLandscape: isLandscape)
        hostingView.navigationItem.hidesBackButton = true
        hostingView.viewName = viewName
        if let navigationController = window.rootViewController as? CustomNavigationController {
            
            swipeScreenOrderList.append(swipeActivation)
            
            // Create the transition animation
            let transition = CATransition()
            transition.duration = 0.3
            transition.timingFunction = CAMediaTimingFunction(name: CAMediaTimingFunctionName.easeInEaseOut)
            transition.type = animationType
            transition.subtype = type
            navigationController.view.layer.add(transition, forKey: kCATransition)
            
            // Get the current navigation stack
            var viewControllers = navigationController.viewControllers
            
            // Remove the last view controller from the stack
            if !viewControllers.isEmpty {
                _ = viewControllers.removeLast()
            }

            // Add the new hosting view controller to the stack
            viewControllers.append(hostingView)
            
            // Set the modified stack to the navigation controller
            navigationController.setViewControllers(viewControllers, animated: false) // Set animated to false because we're handling the animation with CATransition
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
    func push<T: View>(_ view: T, animated _: Bool, type: CATransitionSubtype, animationType: CATransitionType = .moveIn, swipeActivation: Bool = true, viewName: ViewName) {
        let hostingView = UIHostingController(rootView: view.environment(\.navigationController, self).navigationBarHidden(true).ignoresSafeArea(.all))
        hostingView.navigationItem.hidesBackButton = true
        hostingView.viewName = viewName
        if let navigationController = window.rootViewController as? CustomNavigationController {
            navigationController.isNavigationBarHidden = true
            self.swipeScreenOrderList.append(swipeActivation)
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
    /// 네비게이션 pop 하는 함수 + 화면전환 방향 커스텀 지정
    ///
    /// - Parameters:
    ///    - type ( CATransitionSubtype ) : 화면전환 방향
    /// - Returns:
    ///
    func pop(type: CATransitionSubtype) {
        if let navigationController = window.rootViewController as? CustomNavigationController {
            let transition = CATransition()
            transition.duration = 0.3
            transition.timingFunction = CAMediaTimingFunction(name: CAMediaTimingFunctionName.easeInEaseOut)
            transition.type = CATransitionType.reveal
            transition.subtype = type
            navigationController.view.layer.add(transition, forKey: kCATransition)
            navigationController.popViewController(animated: false)
        }
    }
    
    ///
    /// 네비게이션 pop 하는 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func pop() {
        if let navigationController = window.rootViewController as? CustomNavigationController {
            navigationController.popViewController(animated: true)
        }
    }
    
    ///
    /// 네비게이션 present 하는 함수
    ///
    /// - Parameters:
    ///    - view ( T ) : SwiftUI View
    /// - Returns:
    ///
    func pushPresent<T: View>(_ view: T, animated: Bool, viewName: ViewName) {
        let hostingView = UIHostingController(rootView: view.environment(\.navigationController, self).ignoresSafeArea(.all))
        hostingView.navigationItem.hidesBackButton = true
        hostingView.modalPresentationStyle = .custom
        hostingView.modalTransitionStyle = .coverVertical
        // 커스텀 전환 대리자를 생성하고 적용합니다.
        self.presentController = CustomTransitionDelegate()
        hostingView.transitioningDelegate = self.presentController
        hostingView.isModalInPresentation = false
        hostingView.viewName = viewName
        if let navigationController = window.rootViewController as? CustomNavigationController {
            self.swipeScreenOrderList.append(false)
            navigationController.isNavigationBarHidden = true
            navigationController.present(hostingView, animated: animated, completion: nil)
        }
    }
    
    ///
    /// UIKit 의 UIpresentationController 의 제스처 활성화 유무 설정함수
    ///
    /// - Parameters:
    ///    - viewController ( UIViewController ) : UIKit 의 viewController
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///
    func setPresentControllerSwipeEnable (swipeEnable : Bool) {
        self.presentController?.UIpresentationController?.updateSwipeEnable(swipeEnable: swipeEnable)
    }

    ///
    /// UIKit 의 ViewController 를 직접 present 하는 함수
    ///
    /// - Parameters:
    ///    - viewController ( UIViewController ) : UIKit 의 viewController
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///
    func pushPresent(viewController: UIViewController, animated: Bool, viewName: ViewName) {
        if let navigationController = window.rootViewController as? CustomNavigationController {
            self.swipeScreenOrderList.append(false)
            navigationController.setToolbarHidden(true, animated: true)
            navigationController.setNavigationBarHidden(true, animated: false)
            navigationController.present(viewController, animated: animated)
        }
    }

    
    
    func pushPresent(viewController: UIViewController, animated: Bool) {
         if let rootVC = window.rootViewController as? UINavigationController {
             rootVC.present(viewController, animated: animated)
         }
     }

    
    ///
    /// 네비게이션 present 로 띄어진 스크린을 pop 해주는 함수
    ///
    /// - Parameters:
    /// - Returns:
    ///
    func popPresent() {
        window.rootViewController?.presentedViewController?.dismiss(animated: true, completion: nil)
    }
    
    ///
    /// 네비게이션 최상단 부모 컨트롤러로 한번에 pop 하는 함수
    ///
    /// - Parameters:
    ///    - animated ( Bool ) : 화면전환 애니메이션 유무
    /// - Returns:
    ///

    func popToRoot(animated: Bool) {
        if let navigationController = window.rootViewController as? CustomNavigationController {
            navigationController.popToRootViewController(animated: animated)
        }
    }
}

class CustomPresentationController: UIPresentationController {
    var swipeGesture: UIPanGestureRecognizer?
    var presentationHeight: CGFloat = UIScreen.main.bounds.height  // 초기 높이 설정
    var swipeEnable: Bool?
    override init(presentedViewController: UIViewController, presenting presentingViewController: UIViewController?) {
        super.init(presentedViewController: presentedViewController, presenting: presentingViewController)
        setupSwipeGesture()
    }
    
    override func containerViewWillLayoutSubviews() {
        super.containerViewWillLayoutSubviews()
        presentedView?.frame = frameOfPresentedViewInContainerView
    }
    
    // 높이 변경 메소드
    public func updateHeight(to newHeight: CGFloat) {
        presentationHeight = newHeight
        UIView.animate(withDuration: 0.3) { [weak self] in
            self?.containerView?.setNeedsLayout()
            self?.containerView?.layoutIfNeeded()
        }
    }
    
    public func updateSwipeEnable (swipeEnable: Bool) {
        self.swipeEnable = swipeEnable
        if swipeEnable {
            setupSwipeGesture()
        } else {
            if let swipeGesture = swipeGesture {
                presentedViewController.view.removeGestureRecognizer(swipeGesture)
            }
        }
    }
    
    private func setupSwipeGesture() {
        let gesture = UIPanGestureRecognizer(target: self, action: #selector(handleSwipe(_:)))
               presentedViewController.view.addGestureRecognizer(gesture)
               swipeGesture = gesture // 제스처 참조 저장
    }
    
    @objc private func handleSwipe(_ gesture: UIPanGestureRecognizer) {
        let translation = gesture.translation(in: presentedViewController.view)
        let velocity = gesture.velocity(in: presentedViewController.view)
        
        if gesture.state == .ended {
            if let swipeEnable = self.swipeEnable {
                if !swipeEnable { return }
            }
            
            if translation.y > 100 && velocity.y > 0 {
                // 여기서 닫기 동작을 구현
                presentedViewController.dismiss(animated: true, completion: nil)
            }
        }
    }
    
  override var frameOfPresentedViewInContainerView: CGRect {
    // 컨테이너 뷰 내에서 프레젠테이션된 뷰의 프레임을 정의합니다.
    guard let containerView = containerView else { return .zero }
    return CGRect(x: 0, y: containerView.bounds.height - self.presentationHeight, width: containerView.bounds.width, height: self.presentationHeight)
  }
    
  override func presentationTransitionWillBegin() {
           super.presentationTransitionWillBegin()

           // 프레젠테이션된 뷰의 레이어에 접근하여 cornerRadius를 설정
           presentedView?.layer.cornerRadius = 20
           presentedView?.clipsToBounds = true
  }
}

class CustomTransitionDelegate: NSObject, UIViewControllerTransitioningDelegate {
    public var UIpresentationController:CustomPresentationController?
    func presentationController(forPresented presented: UIViewController, presenting: UIViewController?, source: UIViewController) -> UIPresentationController? {
        // CustomPresentationController 인스턴스를 생성합니다.
        self.UIpresentationController = CustomPresentationController(presentedViewController: presented, presenting: presenting)
        return self.UIpresentationController
    }
}

extension NavigationController: UIGestureRecognizerDelegate {
    public func gestureRecognizerShouldBegin(_ gestureRecognizer: UIGestureRecognizer) -> Bool {
        // 가로 스와이프인지 확인
        if let panGestureRecognizer = gestureRecognizer as? UIPanGestureRecognizer, let view = panGestureRecognizer.view {
            let velocity = panGestureRecognizer.velocity(in: view)
            let isHorizontalSwipe = abs(velocity.x) > abs(velocity.y)
            
            // 스와이프가 가로 방향일 때만 제스처를 허용
            if isHorizontalSwipe {
                if self.swipeScreenOrderList.isEmpty {
                    return false
                } else {
                    return self.swipeScreenOrderList.last!
                }
            } else {
                return false
            }
        }
        return false
    }
}

extension NavigationController: UINavigationControllerDelegate {
    func navigationController(_ navigationController: UINavigationController, didShow viewController: UIViewController, animated: Bool) {
        // 네비게이션 스택의 이전 뷰 컨트롤러를 확인
        if let previousViewController = navigationController.transitionCoordinator?.viewController(forKey: .from),
            !navigationController.viewControllers.contains(previousViewController) {
            // 여기서 pop 이벤트를 처리
            let viewName = previousViewController.viewName ?? .None
        }
    }
}


extension UIViewController {
    
    @objc func shouldPush() -> Bool {
        // 기본적으로 true를 반환하지만, 서브클래스에서 이 메서드를 오버라이드하여 특정 조건을 확인할 수 있습니다.
        return true
    }
    
    var viewName: ViewName? {
        get {
            return objc_getAssociatedObject(self, &AssociatedObjectHandle) as? ViewName
        }
        set {
            objc_setAssociatedObject(self, &AssociatedObjectHandle, newValue, .OBJC_ASSOCIATION_RETAIN_NONATOMIC)
        }
    }
}


class CustomNavigationController: UINavigationController, UINavigationControllerDelegate {
    var shouldPushViewController: ((UIViewController) -> Bool)?
    
    override var supportedInterfaceOrientations: UIInterfaceOrientationMask {
        return topViewController?.supportedInterfaceOrientations ?? .portrait
    }

    override var shouldAutorotate: Bool {
        topViewController?.shouldAutorotate ?? true
    }
    
    override func pushViewController(_ viewController: UIViewController, animated: Bool) {
         if let shouldPush = shouldPushViewController {
             if !shouldPush(viewController) {
                 return
             }
         } else if !viewController.shouldPush() {
             return
         }
         
         // 이전 뷰 컨트롤러의 viewName과 새 뷰 컨트롤러의 viewName을 비교
         if let topViewController = self.topViewController,
            let topViewName = topViewController.viewName,
            let newViewName = viewController.viewName,
            topViewName == newViewName {
             if DoublePushViewName(rawValue: newViewName.rawValue) != nil {
                 return // newViewName이 DoublePushViewName에 포함되면 push를 하지 않음
             }
         }
         
         super.pushViewController(viewController, animated: animated)
     }
}


class CustomHostingController<Content>: UIHostingController<Content> where Content: View {
    var isLandscape: Bool

    init(rootView: Content, isLandscape: Bool) {
        self.isLandscape = isLandscape
        super.init(rootView: rootView)
    }
    
    @objc required dynamic init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override var supportedInterfaceOrientations: UIInterfaceOrientationMask {
        return isLandscape ? .portrait : .portrait
    }

    override var shouldAutorotate: Bool {
        return false
    }
}
