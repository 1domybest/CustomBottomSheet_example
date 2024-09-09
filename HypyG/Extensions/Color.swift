//
//  Color.swift
//  HypyG
//
//  Created by romi on 1/12/24.
//

import Foundation
import SwiftUI


///
/// Color extension
///
extension Color {
    ///
    /// 원하는 컬러 생성
    ///
    ///  사용 방법 :
    ///
    /// ```
    /// Text("안녕하세요, SwiftUI!")
    ///   .font(.title)
    ///   .foregroundColor(Color.gray6)
    /// ```
    ///
    ///  공지 :
    ///     static let black0 = "#000000".hexToColor()! //hex: "#000000"
    ///     위처럼 Assets에 등록되어있지 않은경우 아직 진왕님이 지정하지않은 컬러
    ///
    
    // 미정
    static let black0 = "#000000".hexToColor()! //hex: "#000000"
    static let pink0 = "#FFB2EC".hexToColor()! //hex: "#FFB2EC"
    static let pink1 = "#E52CB6".hexToColor()! //hex: "#E52CB6"
    static let green0 = "#AEE652".hexToColor()! //hex: "#8EDB11"
    static let green1 = "#8EDB11".hexToColor()! //hex: "#8EDB11"
    static let blue0 = "#92B0FF".hexToColor()! //hex: "#4A7DFF"
    static let blue1 = "#4A7DFF".hexToColor()! //hex: "#4A7DFF"
    static let skyblue0 = "#B8BFFF".hexToColor()! //hex: "#B8BFFF"
    static let mint0 = "#85E9E3".hexToColor()! //hex: "#46D6CD"
    static let mint1 = "#46D6CD".hexToColor()! //hex: "#46D6CD"
    static let orange0 = "#FFC9A1".hexToColor()! //hex: "#FF964A"
    static let orange1 = "#FF964A".hexToColor()! //hex: "#FF964A"
    static let red0 = Color("Red") //hex: "#FF0000"
    static let white0 = Color("White") //hex: "#FFFFFF"
    static let blueViolet0 = Color("BlueViolet0") //hex: "#A686FF"
    static let blueViolet1 = Color("BlueViolet1") //hex: "#794AFF"
   
    // 네비게이션바 색
    static let naviGray = "#0109164D".hexToColor()! //hex: "#010916" 30% => 0109164D
    
    // 미션 정보 라인
    static let missionInfoLine = "#4AD0FF".hexToColor()!
    
    // 랭킹
    static let rankFontColor = Color.gray7
    static let rankBgColor = Color.gray0
    static let rankBorderColor = Color.gray4
    
    // 스킬
    static let rewardSkillFontColor = "#01A3FF".hexToColor()! // hex: "#01A3FF"
    static let rewardSkillBgColor = "#C4E5F7".hexToColor()! // hex: "#01A3FF33" -> #C4E5F7
    
    // 캐릭터 성향
    static let rewardCharacterFontColor = "#794AFF".hexToColor()! // hex: "#794AFF"
    static let rewardCharacterBgColor = "#DCD3F7".hexToColor()! // hex: "#794AFF33" -> #DCD3F7
    
    // 재화
    static let hPointFontColor = "#FF5C00".hexToColor()! // hex: "#FF5C00"
    static let hPointBgColor = "#FFC70033".hexToColor()! // hex: "#FFC70033"
    static let hPointBorderColor = "#FFC700".hexToColor()! // hex: "#FFC700"
    
    // 충전하기 - 이벤트 색
    static let productEventBorderColor = "#FFC700".hexToColor()! // hex: "#FFC700"
    static let productEventBgColor = "#FFF4CC".hexToColor()! // hex: "#FFF4CC"
    
    // 아이템 샵 - 투표권
    static let shopEventBgColor = "#FFF4CC".hexToColor()! // hex: "#FFF4CC"
    
    // 회원가입 배경색
    static let signUpIdolBgColor = "#4D80E6".hexToColor()! // hex: "#4D80E6"
    
    // 회원가입 - 직업 선택 문구 색
    static let jobIdolIntroFont = "#01A3FF".hexToColor()! // hex: "#01A3FF"
    static let jobProducerIntroFont = "#794AFF".hexToColor()! // hex: "#794AFF"
    
    // 스킬 탭 태그 색상
    static let skillTagBackgroundColor = "#CCEDFF".hexToColor()! // hex: "#CCEDFF"
    static let skillTagFontColor = "#01A3FF".hexToColor()! // hex: "#01A3FF"
    
    // 스킬
    static let depositFontColor = "#01A3FF".hexToColor()! // hex: "#01A3FF"
    static let withdrawalFontColor = "#FF0000".hexToColor()! // hex: "#FF0000"
    
    // 회원가입시에 사용하는 블루 칼러
    static let hypyBlueColor = "#2F88FF".hexToColor()! // hex: "#2F88FF"
    
    // 지정완료
    static let gray0 = Color("Gray0") //hex: "#f8f9fa"
    static let gray1 = Color("Gray1") //hex: "#f1f3f5"
    static let gray2 = Color("Gray2") //hex: "#e9ecef"
    static let gray3 = Color("Gray3") //hex: "#dee2e6"
    static let gray4 = Color("Gray4") //hex: "#ced4da"
    static let gray5 = Color("Gray5") //hex: "#adb5bd"
    static let gray6 = Color("Gray6") //hex: "#868e96"
    static let gray7 = Color("Gray7") //hex: "#495057"
    static let gray8 = Color("Gray8") //hex: "#343a40"
    static let gray9 = Color("Gray9") //hex: "#212529"
    
    func getUIColor () -> UIColor {
        return UIColor(self)
    }
}
