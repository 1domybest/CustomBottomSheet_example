//
//  File.swift
//  HypyG
//
//  Created by 온석태 on 12/26/23.
//

import Foundation
import UIKit

extension Int {
    // 기준 가로와 세로 크기를 바탕으로 현재 화면 대각선에 대한 비율을 계산하는 함수
     func scaledDiagonal(basedOn standardSize: CGSize = CGSize(width: 393, height: 852)) -> CGFloat {
         // 기준 크기의 대각선 길이 계산
         let standardDiagonal = sqrt(pow(standardSize.width, 2) + pow(standardSize.height, 2))

         // 현재 화면 크기의 대각선 길이 계산
         let screenWidth = UIScreen.main.bounds.width
         let screenHeight = UIScreen.main.bounds.height
         
         let currentDiagonal = sqrt(pow(screenWidth, 2) + pow(screenHeight, 2))
         
         // 비율 계산
         return CGFloat(self) * currentDiagonal / standardDiagonal
     }
    
    func getWidthScaledDiagonal(basedWidth: CGFloat = 393) -> CGFloat {
        let screenWidth = UIScreen.main.bounds.width
        let ratio = CGFloat(self) / basedWidth
        
        return screenWidth * ratio
    }
    
    func getHeightScaledDiagonal(basedWidth: CGFloat = 852) -> CGFloat {
        let screenHeight = UIScreen.main.bounds.height
        let ratio = CGFloat(self) / basedWidth
        
        return screenHeight * ratio
    }
}
