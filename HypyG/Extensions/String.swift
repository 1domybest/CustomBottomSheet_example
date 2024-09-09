//
//  String.swift
//  HypyG
//
//  Created by 온석태 on 11/25/23.
//

import Foundation
import UIKit
import SwiftUI
///
/// String extension
///
/// - Parameters:
///  - path : 파일이름 -> 디바이스 내 파일 경로로 변환
/// - Returns:
///
extension String {
    // 파일이름(확장자) 디바이스 내부 경로 리턴
    var path: String? {
        return Bundle.main.path(forResource: self, ofType: nil)
    }
    
    func width(for font: UIFont) -> CGFloat {
        let fontAttributes = [NSAttributedString.Key.font: font]
        let size = self.size(withAttributes: fontAttributes)
        let scale = UIScreen.main.scale
        return size.width
    }
    
    
    func markdownToHTML() -> String {
           if let data = self.data(using: .utf8) {
               do {
                   let attributedString = try NSAttributedString(
                       data: data,
                       options: [
                           .documentType: NSAttributedString.DocumentType.html,
                           .characterEncoding: String.Encoding.utf8.rawValue
                       ],
                       documentAttributes: nil
                   )
                   return attributedString.string
               } catch {
                   print("Error creating attributed string: \(error)")
               }
           }
           return ""
       }
    
    func hexToColor() -> Color? {
        // # 문자 제거
        var hexSanitized = self.trimmingCharacters(in: .whitespacesAndNewlines)
        hexSanitized = hexSanitized.replacingOccurrences(of: "#", with: "")

        var rgb: UInt64 = 0
        Scanner(string: hexSanitized).scanHexInt64(&rgb)
        
        var red: Double = 0
        var green: Double = 0
        var blue: Double = 0
        var opacity: Double = 1.0
        if hexSanitized.count == 6 {
            red = Double((rgb & 0xFF0000) >> 16) / 255.0
            green = Double((rgb & 0x00FF00) >> 8) / 255.0
            blue = Double(rgb & 0x0000FF) / 255.0
        } else {
            red = Double((rgb >> 24) & 0xFF) / 255.0
            green = Double((rgb >> 16) & 0xFF) / 255.0
            blue = Double((rgb >> 8) & 0xFF) / 255.0
            opacity = Double(rgb & 0xFF) / 255.0
        }
        return Color(red: red, green: green, blue: blue, opacity: opacity)
    }
    
    func toUIColor(withAlpha alpha: CGFloat = 1.0) -> UIColor? {
           var hexString = self.trimmingCharacters(in: .whitespacesAndNewlines)
           hexString = hexString.replacingOccurrences(of: "#", with: "")

           var rgbValue: UInt64 = 0
           Scanner(string: hexString).scanHexInt64(&rgbValue)

           let red = CGFloat((rgbValue & 0xFF0000) >> 16) / 255.0
           let green = CGFloat((rgbValue & 0x00FF00) >> 8) / 255.0
           let blue = CGFloat(rgbValue & 0x0000FF) / 255.0

           return UIColor(red: red, green: green, blue: blue, alpha: alpha)
       }
    
    /// 예상 높이
    func expectedHeight(withWidth width: CGFloat, font: UIFont) -> CGFloat {
        let constraintRect = CGSize(width: width, height: .greatestFiniteMagnitude)
        let boundingBox = self.boundingRect(with: constraintRect, options: [.usesFontLeading, .usesLineFragmentOrigin], attributes: [NSAttributedString.Key.font: font], context: nil)
        
        return boundingBox.height
    }
    
    /// 예상 너비
    func expectedWidth(withHeight height: CGFloat, font: UIFont) -> CGFloat {
        let constraintRect = CGSize(width: .greatestFiniteMagnitude, height: height)
        let boundingBox = self.boundingRect(with: constraintRect, options: [.usesFontLeading, .usesLineFragmentOrigin], attributes: [NSAttributedString.Key.font: font], context: nil)
        
        return boundingBox.width
    }
    
    // return CGSize
    func size(withFont font: UIFont) -> CGSize {
        let string = NSString(string: self)
        let size = string.size(withAttributes: [NSAttributedString.Key.font: font])
        return size
    }
    
    func stringToISO8601Date() -> Date? {
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd'T'HH:mm:ss.SSS'Z'"
        dateFormatter.timeZone = TimeZone(secondsFromGMT: 0) // UTC
        dateFormatter.locale = Locale(identifier: "en_US_POSIX") // 로케일 설정
        return dateFormatter.date(from: self)
    }
        
    func stringToServerDate(timeZoon: TimeZone? = nil) -> Date? {
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd HH:mm:ss"
        if timeZoon == nil {
            dateFormatter.timeZone = .current
        } else {
            dateFormatter.timeZone = timeZoon // UTC
        }
        
        dateFormatter.locale = Locale(identifier: "en_US_POSIX") // 로케일 설정
        return dateFormatter.date(from: self)
    }
    
    /// 날짜 스트링을  YYYY. M. D. 로 변환
    /// 2024-03-28 01:52:06.878 -> 2024. 3. 28. 이렇게 변환
    func convertDateStringToYYYYMD() -> String? {
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd HH:mm:ss"
        
        if let date = dateFormatter.date(from: self) {
            let outputDateFormatter = DateFormatter()
            outputDateFormatter.dateFormat = "yyyy. M. d."
            return outputDateFormatter.string(from: date)
        }
        return nil
    }
    
    /// 날짜 스트링을  YYYY. M. D. HH:MM AM/PM 로 변환
    /// 2024-03-28 01:52:06.878 -> 2023. 03. 17. 11:43 이렇게 변환
    func convertDateStringToYYYYMDHMM() -> String? {
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd HH:mm:ss"

        if let date = dateFormatter.date(from: self) {
            let outputDateFormatter = DateFormatter()
            outputDateFormatter.dateFormat = "yyyy. M. d. h:mm a"
            outputDateFormatter.amSymbol = "AM"
            outputDateFormatter.pmSymbol = "PM"
            return outputDateFormatter.string(from: date)
        }
        return nil

    }
    
    /// 날짜 스트링을  YYYY. M. D. 로 변환
    /// 2024-03-28 -> 2024. 3. 28. 이렇게 변환
    func convertDateToYYYYMD() -> String? {
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd"
        
        if let date = dateFormatter.date(from: self) {
            let outputDateFormatter = DateFormatter()
            outputDateFormatter.dateFormat = "yyyy. M. d."
            return outputDateFormatter.string(from: date)
        }
        return nil
    }
    
    /// 날짜 스트링을 YYYY. M. D. 에서 YYYY. MM. DD. 로 변환
    /// 2024. 3. 8. -> 2024. 03. 08. 이렇게 변환
    func convertDateToYYYYMMDD() -> String? {
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy. M. d."
        
        if let date = dateFormatter.date(from: self) {
            let outputDateFormatter = DateFormatter()
            outputDateFormatter.dateFormat = "yyyy. MM. dd."
            return outputDateFormatter.string(from: date)
        }
        return nil
    }
    
    /// D-Day 계산 함수
    func calculateDDay() -> Int? {
        let currentDate = Date()
        let calendar = Calendar.current
        
        // 날짜 형식 지정
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd HH:mm:ss"
        
        if let date = dateFormatter.date(from: self) {
            let fromDateOnly = calendar.date(from: calendar.dateComponents([.year, .month, .day], from: date)) ?? Date()
            let toDateOnly = calendar.date(from: calendar.dateComponents([.year, .month, .day], from: currentDate)) ?? Date()
            
            let components = calendar.dateComponents([.day], from: fromDateOnly, to: toDateOnly)
            return components.day ?? 0
        }
        return nil
    }
    
    /// UI에서 텍스트 width 가져오기
    func getWidthForText(fontType: String = Font.SpoqaHanSansNeo.regular.value, fontSize: CGFloat = 14) -> CGFloat {
        let customFont = UIFont(name: fontType, size: fontSize)!
        let font = UIFontMetrics.default.scaledFont(for: customFont)
        return self.widthOfString(usingFont: font)
    }
    
    /// 문자열 길이(width) 구하기
    func widthOfString(usingFont font: UIFont) -> CGFloat {
        let fontAttributes = [NSAttributedString.Key.font: font]
        let size = self.size(withAttributes: fontAttributes)
        return size.width
    }
    
    /// 문자열 높이(height) 구하기
    func heightOfString(usingFont font: UIFont) -> CGFloat {
        let fontAttributes = [NSAttributedString.Key.font: font]
        let size = self.size(withAttributes: fontAttributes)
        return size.height
    }
    
    func heightOfStringWithN(usingFont font: UIFont, maxWidth: CGFloat) -> CGFloat {
        // 텍스트를 줄바꿈 문자(\n)를 기준으로 나눔
        let lines = self.components(separatedBy: "\n")
        
        // 각 줄의 너비 및 줄 수 계산
        var totalHeight: CGFloat = 0
        for line in lines {
            // 현재 줄의 너비 계산
            var lineWidth = line.size(withAttributes: [NSAttributedString.Key.font: font]).width
            
            // 최대 가로 길이를 넘어가는 경우 줄바꿈을 수행
            if lineWidth > maxWidth {
                var words = line.components(separatedBy: .whitespaces)
                var currentLine = ""
                while !words.isEmpty {
                    let word = words.removeFirst()
                    let wordWidth = word.size(withAttributes: [NSAttributedString.Key.font: font]).width
                    
                    // 현재 단어를 추가하여 너비를 확인
                    let newLineWidth = (currentLine.isEmpty ? 0 : currentLine.size(withAttributes: [NSAttributedString.Key.font: font]).width) + wordWidth
                    
                    // 너비가 최대 가로 길이를 넘어가는 경우 현재 줄을 종료하고 새로운 줄로 시작
                    if newLineWidth > maxWidth {
                        totalHeight += font.lineHeight
                        currentLine = word
                    } else {
                        if !currentLine.isEmpty {
                            currentLine += " "
                        }
                        currentLine += word
                    }
                }
                
                // 남은 단어가 없는 경우 현재 줄의 높이를 추가
                if !currentLine.isEmpty {
                    totalHeight += font.lineHeight
                }
            } else {
                // 최대 가로 길이를 넘지 않는 경우 현재 줄의 높이를 추가
                totalHeight += font.lineHeight
            }
        }
        
        return totalHeight
    }

    
    /// 문자열의 라인수 구하기
    func numberOfLines(using font: UIFont, constrainedToWidth width: CGFloat) -> Int {
        let label = UILabel()
        label.text = self
        label.font = font
        label.numberOfLines = 0
        let maxSize = CGSize(width: width, height: .infinity)
        let size = label.sizeThatFits(maxSize)
        let numberOfLines = Int(size.height / label.font.lineHeight)
        
//        print("폰트 라인갯수 \(numberOfLines)")
        return numberOfLines
    }
    
    /// 글자 단위 줄바꿈
    func splitCharacter() -> String {
        if #available(iOS 16.0, *) {
            return self.split(separator: "").joined(separator: "\u{200B}")
        } else {
            return self.map { String($0) }.joined(separator: "\u{200B}")
        }
    }

    
    func localize(comment: String = "") -> String {
        let defaultLanguage = "ja"
        let value = NSLocalizedString(self, comment: comment)
        
        // Check if the value was localized or if the preferred language is the default language
        if value != self || NSLocale.preferredLanguages.first == defaultLanguage {
            return value.replacingOccurrences(of: "\\n", with: "\n")
        }
        
        // Load resource for the default language to be used as a fallback
        guard let path = Bundle.main.path(forResource: defaultLanguage, ofType: "lproj"),
              let bundle = Bundle(path: path) else {
            return value.replacingOccurrences(of: "\\n", with: "\n")
        }
        
        return NSLocalizedString(self, bundle: bundle, comment: comment).replacingOccurrences(of: "\\n", with: "\n")
    }
    
    func localizedFormattedString(_ data: String) -> String {
        let localizedTemplate = NSLocalizedString(self, tableName: "Localizable", bundle: .main, comment: "")
        
        let message = String(format: localizedTemplate, data)
        return message
    }
    
    func localizedFormattedString(_ data: [String]) -> String {
        let localizedTemplate = NSLocalizedString(self, tableName: "Localizable", bundle: .main, comment: "")
        
        // Use withCString() to pass elements as separate arguments
        let message = withVaList(data) { NSString(format: localizedTemplate, arguments: $0) as String }
                
        return message
    }
    
    func localizedFormattedString(_ data: String, count: Int) -> String {
        let localizedTemplate = NSLocalizedString(self, tableName: "Localizable", bundle: .main, comment: "")
        
        var formatArguments = [CVarArg]()
        for _ in 0..<count {
            formatArguments.append(data)
        }
        
        let message = String(format: localizedTemplate, arguments: formatArguments)
        return message
    }
}
