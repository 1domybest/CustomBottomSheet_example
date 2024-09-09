//
//  RegexPatternEnum.swift
//  HypyG
//
//  Created by 온석태 on 8/30/24.
//

import Foundation

enum RegexPattern: String {
    case onlyEnAndJa = "^[a-zA-Z0-9ぁ-んァ-ン一-龯々〆〤!@#$%^&*()_+{}\\[\\]:;\"'<>,.?/~`\\\\|/\\-]*$"
    case none = "none"
}
