//
//  MyViewController.swift
//  HypyG
//
//  Created by 온석태 on 9/9/24.
//

import UIKit
import SwiftUI

class CustomUIKitBottomSheet: UIViewController {
    
    var customUIKitBottomSheetOption: CustomUIKitBottomSheetOption
    var customModalPresentationController: CustomModalPresentationController?
    
    private var keyboardHeightConstraint: NSLayoutConstraint?
    
    private var initialTouchPoint: CGPoint?
    
    
    var handlerView: UIView?
    var topSafeAreaSize: CGFloat = .zero
    var scrollView: UIScrollView?
    var hostingController: UIHostingController<AnyView>?
    
    var keyboardManager: KeyboardManager?
    var keyboardHeight: CGFloat = .zero
    var isKeyboardOpen: Bool = false
    var lastSizeOfScrollViewContentHeight: CGFloat = .zero
    var currentSizeOfScrollViewContentHeight: CGFloat = .zero
    
    var dragGesture: UIPanGestureRecognizer?
    
    init(bottomSheetModel: CustomUIKitBottomSheetOption) {
        self.customUIKitBottomSheetOption = bottomSheetModel
        // 핸들 가능 모드 custom, automatic , popover, pageSheet, formSheet
        // 핸들 불가능 모드 currentContext, fullScreen, overCurrentContext, overFullScreen,
        super.init(nibName: nil, bundle: nil)
        self.modalPresentationStyle = self.customUIKitBottomSheetOption.sheetMode
        
        if self.customUIKitBottomSheetOption.sheetMode == .custom {
            self.transitioningDelegate = self
        }
        self.keyboardManager = KeyboardManager()
        self.keyboardManager?.setCallback(callback: self)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    deinit {
        print("CustomUIKitBottomSheet deinit")
    }
    
    func unreference () {
        self.customModalPresentationController = nil
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        setView()
        
        if self.customUIKitBottomSheetOption.dragAvailable && self.customUIKitBottomSheetOption.sheetMode == .custom {
            addGestureRecognizers()
        }
        
        if let rootViewController = UIApplication.shared.windows.first?.rootViewController {
            let safeAreaInsets = rootViewController.view.safeAreaInsets
            topSafeAreaSize = safeAreaInsets.top
         }
    }
    
    func setView () {
        if self.customUIKitBottomSheetOption.sheetMode == .custom {
            setSheetView()
        }
        
//        setHostingView()
        setupScrollView()
        
        if self.customUIKitBottomSheetOption.showHandler && self.customUIKitBottomSheetOption.availableHasHandle {
            setHandlerView()
        }
    }
    
    func addGestureRecognizers() {
         let dragGesture = UIPanGestureRecognizer(target: self, action: #selector(handlePanGesture(_:)))
         view.addGestureRecognizer(dragGesture)
        self.dragGesture = dragGesture
     }
     
    @objc func handlePanGesture(_ gesture: UIPanGestureRecognizer) {
        guard let containerView = self.view else { return }

      
        let translation = gesture.translation(in: containerView)
        let velocity = gesture.velocity(in: containerView)

        switch gesture.state {
        case .began:
            // 초기 터치 포인트와 시작 위치 저장
            DispatchQueue.main.async {
                if self.isKeyboardOpen {
                    self.keyboardManager?.hideKeyboard()
                }
                
                self.initialTouchPoint = containerView.frame.origin
            }
           
        case .changed:
            DispatchQueue.main.async {
                if self.isKeyboardOpen { return }
                // 바텀 시트의 새로운 y 위치를 계산
                if let initialTouchPoint = self.initialTouchPoint, translation.y > 0 {
                    // 드래그에 따라 y 위치를 업데이트 (최초 위치 + 드래그 이동 거리)
                    containerView.frame.origin.y = initialTouchPoint.y + translation.y
                }
            }
           

        case .ended, .cancelled:
            DispatchQueue.main.async {
                if self.isKeyboardOpen { return }
                
                let velocityThreshold: CGFloat = 1000 // 속도 기준 값 설정 (점프 스냅에 사용)
                let dismissThreshold: CGFloat = containerView.frame.height / 2 // 화면 하단으로 내려갈 기준 높이
                // 빠르게 드래그하면 바텀 시트를 아래로 dismiss
                if velocity.y > velocityThreshold {
                    self.dismissPresent()
                }
                // 느리게 드래그했을 때는 특정 위치(1/3 지점)를 넘으면 dismiss
                else if translation.y > dismissThreshold {
                    self.dismissPresent()
                }
                // 그렇지 않으면 다시 제자리로 돌아가도록 애니메이션 처리
                else {
                    UIView.animate(withDuration: 0.3) {
                        print("등록된 y2 \(self.initialTouchPoint?.y)")
                        containerView.frame.origin.y = self.initialTouchPoint?.y ?? 0
                    }
                }
            }
           

        default:
            break
        }
        
    }

    func setHostingView () {
        // SwiftUI View를 UIHostingController로 감쌈
        let swiftUIView = self.customUIKitBottomSheetOption.someView

        let hostingController = UIHostingController(rootView: swiftUIView)
        
        // HostingController 추가
        addChild(hostingController) // 자식 ViewController로 추가
        
        hostingController.view.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(hostingController.view) // 뷰 계층에 추가
        
        var topPadding = self.customUIKitBottomSheetOption.showHandler ? 40.0 : 28.0
        
        if self.customUIKitBottomSheetOption.sheetMode != .custom {
            topPadding = 0
        }
        
        // AutoLayout 제약 추가 (UIViewController에 맞추어 SwiftUI 뷰 크기 조절)
        NSLayoutConstraint.activate([
            hostingController.view.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            hostingController.view.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            hostingController.view.topAnchor.constraint(equalTo: view.topAnchor, constant: topPadding),
            hostingController.view.bottomAnchor.constraint(equalTo: view.bottomAnchor)
        ])
        
        hostingController.didMove(toParent: self) // 이동 완료
        
    }
    
    func setupScrollView() {
        let scrollView = CustomScrollView()
        scrollView.backgroundColor = .blue
        scrollView.isScrollEnabled = false
        scrollView.translatesAutoresizingMaskIntoConstraints = false
        scrollView.alwaysBounceVertical = true
        scrollView.showsVerticalScrollIndicator = true
        view.addSubview(scrollView)
        
        var topPadding = self.customUIKitBottomSheetOption.showHandler ? 40.0 : 28.0
        
        if self.customUIKitBottomSheetOption.sheetMode != .custom {
            topPadding = 0
        }
        
        // 오토레이아웃 설정
        NSLayoutConstraint.activate([
            scrollView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            scrollView.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            scrollView.topAnchor.constraint(equalTo: view.topAnchor, constant: topPadding),
            scrollView.bottomAnchor.constraint(equalTo: view.bottomAnchor),
            scrollView.widthAnchor.constraint(equalTo: view.widthAnchor)  // 가로 스크롤 방지
        ])
        
        // SwiftUI View를 AnyView로 감싸서 사용
        let swiftUIView = self.customUIKitBottomSheetOption.someView
        
        let hostingController = UIHostingController(rootView: AnyView(swiftUIView))
        hostingController.view.frame = CGRect(x: 0, y: 0, width: view.frame.width, height: self.customUIKitBottomSheetOption.sheetHeight)
        
        scrollView.addSubview(hostingController.view)
    
        addChild(hostingController)
        hostingController.didMove(toParent: self)
        self.hostingController = hostingController
        self.scrollView = scrollView
    }
    
    func setSheetView () {
        view.backgroundColor = self.customUIKitBottomSheetOption.sheetColor.getUIColor()
        
        let radius: CGFloat = 20.0 // 원하는 radius 값으로 변경
        let corners: UIRectCorner = [.topLeft, .topRight]
        let path = UIBezierPath(roundedRect: view.bounds, byRoundingCorners: corners, cornerRadii: CGSize(width: radius, height: radius))
        let maskLayer = CAShapeLayer()
        maskLayer.path = path.cgPath
        
        self.view.layer.mask = maskLayer
    }
    
    func setHandlerView() {
        let handlerView = UIView()
        handlerView.backgroundColor = self.customUIKitBottomSheetOption.handlerColor.getUIColor()
        handlerView.layer.cornerRadius = 2

        view.addSubview(handlerView)
        handlerView.translatesAutoresizingMaskIntoConstraints = false
        
        var topPadding = 8.0
        if self.customUIKitBottomSheetOption.sheetMode != .custom {
            topPadding = 12.0
        }
        
        NSLayoutConstraint.activate([
            handlerView.widthAnchor.constraint(equalToConstant: 36),
            handlerView.heightAnchor.constraint(equalToConstant: 4),
            handlerView.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            handlerView.topAnchor.constraint(equalTo: view.topAnchor, constant: topPadding)
        ])
        
        self.handlerView = handlerView
    }

    
    func dismissPresent () {
        self.dismiss(animated: true)
    }
    
    func updateSheetHeight(newHeight: CGFloat) {
        print("새로운 높이\(newHeight)") // 705
        self.currentSizeOfScrollViewContentHeight = newHeight
        var newHeight = newHeight
        
        // 이곳에서 max확인해야함
        let topPadding = self.customUIKitBottomSheetOption.showHandler ? 40.0 : 28.0
        newHeight += topPadding
        
        var adjustedLength = min(max(newHeight, self.customUIKitBottomSheetOption.minimumHeight), self.customUIKitBottomSheetOption.maximumHeight)
        
        var contentSize:CGFloat = adjustedLength
        print("1계산된 높이\(adjustedLength)")
        if newHeight > UIScreen.main.bounds.height - (topSafeAreaSize + keyboardHeight) {
            // 넘어섰을때
            adjustedLength = UIScreen.main.bounds.height - topSafeAreaSize
            if keyboardHeight > 0 {
                adjustedLength = UIScreen.main.bounds.height - (keyboardHeight + topSafeAreaSize)
            }
            
            print("2계산된 높이\(adjustedLength)")
            
            contentSize = newHeight

//            self.handlerView?.isHidden = true
            self.scrollView?.isScrollEnabled = true
        } else {
            self.handlerView?.isHidden = false
            self.scrollView?.isScrollEnabled = false
        }

        customModalPresentationController?.setSheetHeight(sheetHeight: adjustedLength)
        
        DispatchQueue.main.async {
            self.scrollView?.contentSize = CGSize(width: self.view.frame.width, height: contentSize) // 테스트를 위한 고정된 크기
            self.hostingController?.view.frame.size = CGSize(width: self.view.frame.width, height: contentSize)
            
            print("콘텐트 높이 \(self.scrollView?.contentSize.height) 호스팅뷰 \(self.hostingController?.view.frame)")
        }
        

        DispatchQueue.main.async {
            self.view.layoutIfNeeded()
        }
    }
}

extension CustomUIKitBottomSheet:UIViewControllerTransitioningDelegate {
    func presentationController(forPresented presented: UIViewController, presenting: UIViewController?, source: UIViewController) -> UIPresentationController? {
        self.customModalPresentationController = CustomModalPresentationController(presentedViewController: presented, presenting: presenting, bottomSheetModel: self.customUIKitBottomSheetOption)
        return self.customModalPresentationController
    }
}


extension CustomUIKitBottomSheet:KeyboardManangerProtocol {
    func keyBoardWillShow(notification: NSNotification, keyboardHeight: CGFloat) {
        DispatchQueue.main.async {
            self.isKeyboardOpen = true
            self.lastSizeOfScrollViewContentHeight = self.currentSizeOfScrollViewContentHeight
            self.keyboardHeight = keyboardHeight
            self.customModalPresentationController?.setKeyboardHeight(keyboardHeight: keyboardHeight)
            self.updateSheetHeight(newHeight: self.lastSizeOfScrollViewContentHeight)
        }
        
    }
    
    func keyBoardWillHide(notification: NSNotification) {
        DispatchQueue.main.async {
            self.keyboardHeight = 0
            self.customModalPresentationController?.setKeyboardHeight(keyboardHeight: self.keyboardHeight)
            self.updateSheetHeight(newHeight: self.lastSizeOfScrollViewContentHeight)
            self.isKeyboardOpen = false
        }
        
    }
}

class CustomScrollView: UIScrollView {
    // 스크롤뷰가 스크롤이 비활성화된 상태에서 터치 이벤트를 하위 뷰로 전달하도록 설정
    override func touchesShouldCancel(in view: UIView) -> Bool {
        return true
    }
    
    override func hitTest(_ point: CGPoint, with event: UIEvent?) -> UIView? {
        // 스크롤이 비활성화된 상태에서는 스크롤뷰 자체가 아닌 내부의 뷰가 이벤트를 받을 수 있도록 처리
        if !self.isScrollEnabled {
            return self.subviews.first?.hitTest(point, with: event)
        }
        return super.hitTest(point, with: event)
    }
}
