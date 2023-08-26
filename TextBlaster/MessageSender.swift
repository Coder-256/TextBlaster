//
//  MessageSender.swift
//  TextBlaster
//
//  Created by Jacob Greenfield on 8/26/23.
//

import Foundation
import ScriptingBridge

enum MessageSendError: LocalizedError {
    case appNotFound
    case success

    var errorDescription: String? {
        switch self {
        case .appNotFound:
            return "Messages app not found."
        case .success:
            return "Done! NOTE: you should check for \"Message Failed to Send\" errors."
        }
    }
}

func send(phoneNumbers phoneNumberString: String, message: String) -> [Error] {
    guard let messagesApp = SBApplication(bundleIdentifier: "com.apple.MobileSMS") else {
        return [MessageSendError.appNotFound]
    }
    let phoneNumbers = phoneNumberString
        .split(whereSeparator: { $0.isNewline || $0 == "," })
        .map({ $0.trimmingCharacters(in: .whitespaces) })
    let errors = MessagesHelper(application: messagesApp).sendMessage(message, toPhoneNumbers: phoneNumbers)
    return errors.isEmpty ? [MessageSendError.success] : errors
}
