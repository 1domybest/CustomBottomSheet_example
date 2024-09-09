# Uncomment the next line to define a global platform for your project
# platform :ios, '9.0'

post_install do |installer|
  # ios deployment version
  installer.pods_project.targets.each do |target|
    target.build_configurations.each do |config|
      xcconfig_relative_path = "Pods/Target Support Files/#{target.name}/#{target.name}.#{config.name}.xcconfig"
      file_path = Pathname.new(File.expand_path(xcconfig_relative_path))
      next unless File.file?(file_path)
      configuration = Xcodeproj::Config.new(file_path)
      next if configuration.attributes['LIBRARY_SEARCH_PATHS'].nil?
      configuration.attributes['LIBRARY_SEARCH_PATHS'].sub! 'DT_TOOLCHAIN_DIR', 'TOOLCHAIN_DIR'
      configuration.save_as(file_path)

      # Starscream 타겟의 최소 배포 버전 설정 변경
      if target.name == 'Starscream'
        config.build_settings['IPHONEOS_DEPLOYMENT_TARGET'] = '12.0'
      end

      # Socket.IO-Client-Swift 타겟의 최소 배포 버전 설정 변경
      if target.name == 'Socket.IO-Client-Swift'
        config.build_settings['IPHONEOS_DEPLOYMENT_TARGET'] = '12.0'
      end

      if target.name == 'CropViewController'
        config.build_settings['IPHONEOS_DEPLOYMENT_TARGET'] = '12.0'
      end

      if target.name == 'CropViewController-TOCropViewControllerBundle'
        config.build_settings['IPHONEOS_DEPLOYMENT_TARGET'] = '12.0'
      end


      # 여기에 다른 모든 설정 변경 사항을 추가합니다.
      # 예를 들어, 여기서 'YOUR_SETTING'을 'YOUR_VALUE'로 설정합니다.
      # 이 설정은 모든 타겟에 적용됩니다. 특정 타겟에만 적용하고 싶다면 위의 if 문 내에 넣으세요.
      #config.build_settings['YOUR_SETTING'] = 'YOUR_VALUE'
    end
  end
end


target 'HypyG' do
  # Comment the next line if you don't want to use dynamic frameworks
  use_frameworks!

  # Pods for HypyG
  pod 'SwiftUIIntrospect', '~> 1.0'
  pod 'CropViewController'
  pod 'DeepAR', '5.5.2'
  pod 'SwiftLint'
  pod 'SwiftFormat/CLI'
  pod 'GoogleSignIn'
  pod 'GoogleSignInSwiftSupport'
  pod 'LineSDKSwift', '~> 5.0'
  pod 'Socket.IO-Client-Swift', '~> 16.1.0  '
  pod 'Alamofire'
  pod 'Kingfisher', '~> 7.0'
  pod 'SkeletonUI'
  pod 'FirebaseAnalytics'
  pod 'FirebaseMessaging'
  pod 'MobileVLCKit'
  pod 'RealmSwift'
  pod 'KeychainAccess'
  pod 'SnapKit'
end
