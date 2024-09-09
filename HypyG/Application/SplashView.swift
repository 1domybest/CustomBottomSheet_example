//
//  SplashView.swift
//  HypyG
//
//  Created by romi on 2/8/24.
//

import SwiftUI

struct SplashView: View {

    var body: some View {
        ZStack {
            AsyncImageView(urlString: "https://www.docdocdoc.co.kr/news/photo/201408/157794_53420_0313.jpg")
                .scaledToFill()
                .ignoresSafeArea(.all)
        }
    }
    
}
