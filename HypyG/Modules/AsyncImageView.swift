//
//  AsyncImageView.swift
//  HypyG
//
//  Created by romi on 3/11/24.
//

import SwiftUI
import Kingfisher

///
/// 비동기로 이미지를 다운로드해서 화면에 보여주기
///
/// 사용 방법:
/// ```swift
/// AsyncImageView(urlString: "https:// ~~~~")
/// ```

struct AsyncImageView: View {
    var urlString: String
    
    init(urlString: String) {
        self.urlString = urlString
    }
    
    var body: some View {
        KFImage(URL(string: urlString))
            .placeholder {
                Rectangle()
                    .frame(minWidth: 0, maxWidth: .infinity, minHeight: 0, maxHeight: .infinity)
                    .foregroundColor(Color.gray0)
            }
            .loadDiskFileSynchronously(true)
            .cancelOnDisappear(true)
            .resizable()
    }
}

//struct AsyncImageView: View {
//    @ObservedObject var VM:AsyncImageViewModel
//    init(urlString: String) {
//        VM = AsyncImageViewModel(urlString: urlString)
//
//    }
//
//    var body: some View {
//        if let image = VM.image {
//            Image(uiImage: image)
//                .resizable()
//                .scaledToFit()
//        } else {
//            if self.VM.urlString.isEmpty || self.VM.urlString == "" {
//                // URL이 유효하지 않을 경우 기본 이미지 사용
//                Image(systemName: "photo")
//            } else {
//                // 이미지가 다운로드되지 않은 경우 로딩 표시
//                Rectangle()
//                    .frame(minWidth: 0, maxWidth: .infinity, minHeight: 0, maxHeight: .infinity)
//                    .foregroundColor(Color.gray0)
//                    .onAppear {
//
//                    }
//            }
//        }
//    }
//
//    private func downloadImage(urlString: String) {
//        guard let url = URL(string: urlString) else { return }
//
//        // 이미지 다운로드를 백그라운드에서 수행
//        DispatchQueue.global().async {
//            do {
//                let imageData = try Data(contentsOf: url)
//                if let uiImage = UIImage(data: imageData) {
//                    // 다운로드 완료된 이미지를 메인 스레드에서 업데이트
//                    DispatchQueue.main.async {
//                        self.VM.image = uiImage
//                    }
//                }
//            } catch {
//                print("Error downloading image: \(error)")
//            }
//        }
//    }
//}

//class AsyncImageViewModel: ObservableObject {
//    @Published var image: UIImage?
//    var urlString: String = ""
//
//
//    init(image: UIImage? = nil, urlString: String) {
//        self.image = image
//        self.urlString = urlString
//
//
//        NSCacheManager.shader.load(from: urlString) { result in
//            switch result {
//            case.success(let imagePath):
//                self.image = NSCacheManager.shader.getImage(from: imagePath as String)
//            case.failure(let error):
//                print("loadImage error \(error.localizedDescription)")
//            }
//        }
//    }
//}
