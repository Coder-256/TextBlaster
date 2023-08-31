//
//  MessageSender.swift
//  TextBlaster
//
//  Created by Jacob Greenfield on 8/26/23.
//

import Foundation
import AppKit
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

func sendBlast(message: String, phoneNumberString: String) -> [Error] {
    guard let messagesApp = SBApplication(bundleIdentifier: "com.apple.MobileSMS") else {
        return [MessageSendError.appNotFound]
    }
    let phoneNumbers = phoneNumberString
        .split(whereSeparator: { $0.isNewline || $0 == "," })
        .map({ $0.trimmingCharacters(in: .whitespaces) })
        .filter({ !$0.isEmpty })
    let alert = NSAlert()
    alert.messageText = "Confirm text blast"
    alert.informativeText = "Are you sure you want to send this message to \(phoneNumbers.count) recipient\(phoneNumbers.count == 1 ? "" : "s")?"
    alert.addButton(withTitle: "OK")
    alert.addButton(withTitle: "Cancel")
    if alert.runModal() != .alertFirstButtonReturn {
        return []
    }
    let errors = MessagesHelper(application: messagesApp).sendBlast(message, toPhoneNumbers: phoneNumbers)
    return errors.isEmpty ? [MessageSendError.success] : errors
}
