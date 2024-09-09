//
//  SceneDelegate.swift
//  SwiftUIWithUIKit
//
//  Created by 온석태 on 2023/10/19.
//

import CallKit
import UIKit
import LineSDK

class SceneDelegate: UIResponder, UIWindowSceneDelegate {
    // callObserver = 최상단 AppDelegate 혹은 SceneDelegate 에서 선언해야 정상동작함
    var callObserver: CXCallObserver?
    
    var window: UIWindow?
    
    private lazy var navigationController = NavigationController(window: window!)
    
    /**
     씬(= 앱)이 처음 시작할 때 실행되는 코드
     
     - Parameters:
        - scene: UIScene
        - willConnectTo session: UISceneSession
        - options connectionOptions: UIScene.ConnectionOptions
     - Returns: Void
     */
    func scene(_ scene: UIScene, willConnectTo _: UISceneSession, options connectionOptions : UIScene.ConnectionOptions) {
        guard let windowScene = scene as? UIWindowScene else { return }
        
        
        window = UIWindow(windowScene: windowScene)
        navigationController.showRootView()
        window?.makeKeyAndVisible()
        
        if let rootViewController = UIApplication.shared.windows.first?.rootViewController {
            let safeAreaInsets = rootViewController.view.safeAreaInsets
         }
        let screenSize = UIScreen.main.bounds.size
        let screenWidth = screenSize.width
        let screenHeight = screenSize.height
        
        callObserver = CXCallObserver()
      
        /// Dynamic Link (앱이 종료된 상태)
        if let urlContext = connectionOptions.urlContexts.first {
            print("urlContext = \(urlContext)")
            let sendingAppID = urlContext.options.sourceApplication
            print("source application = \(sendingAppID ?? "Unknown")")
            let url = urlContext.url
            print("url = \(url)")
            
            // TODO : 연결 추가
        }
        
    }


    /// Dynamic Link (앱이 실행 중인 상태)
    func scene(_ scene: UIScene, openURLContexts URLContexts: Set<UIOpenURLContext>) {
        guard let url = URLContexts.first?.url else { return }
        guard url.scheme == "hypyapp" else { return }
        
        // 라인
        _ = LoginManager.shared.application(.shared, open: URLContexts.first?.url)
    }
    
    /**
     씬(= 앱)이 활성 상태가 될 때 실행되는 코드
     
     - Parameters:
        - scene: UIScene
     - Returns: Void
     */
    func sceneDidBecomeActive(_ scene: UIScene) {
    }

    /**
     씬(= 앱)이 비활성 상태로 전환될 때 실행되는 코드
     
     - Parameters:
        - scene: UIScene
     - Returns: Void
     */
    func sceneWillResignActive(_ scene: UIScene) {
    }

    /**
     씬(= 앱)이 백그라운드 상태가 될 때 실행되는 코드
     
     - Parameters:
        - scene: UIScene
     - Returns: Void
     */
    func sceneDidEnterBackground(_ scene: UIScene) {
    }

    /**
     씬(= 앱)이 포어그라운드 상태로 전환될 때 실행되는 코드
     
     - Parameters:
        - scene: UIScene
     - Returns: Void
     */
    func sceneWillEnterForeground(_ scene: UIScene) {
    }

    /**
     씬(= 앱)이 파괴될 때 실행되는 코드
     
     - Parameters:
        - scene: UIScene
     - Returns: Void
     */
    func sceneDidDisconnect(_ scene: UIScene) {
    }
}
