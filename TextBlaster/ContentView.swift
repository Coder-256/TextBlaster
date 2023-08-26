//
//  ContentView.swift
//  TextBlaster
//
//  Created by Jacob Greenfield on 8/26/23.
//

import SwiftUI
import Combine

class ErrorInstance: Identifiable {
    let error: Error

    init(error: Error) {
        self.error = error
    }
}

class SendNotifier: ObservableObject {
    @Published var errors: [ErrorInstance] = []
    @Published var inProgress = false
}

// Based on: https://betterprogramming.pub/implementing-swiftui-onchange-support-for-ios13-577f9c086c9
extension View {
    /// A backwards compatible wrapper for iOS 14 `onChange`
    @ViewBuilder func valueChanged<T: Equatable>(value: T, onChange: @escaping (T) -> Void) -> some View {
        if #available(macOS 13.0, *) {
            self.onChange(of: value, perform: onChange)
        } else {
            self.onReceive(Just(value)) { (value) in
                onChange(value)
            }
        }
    }
}

struct ContentView: View {
    @State var phoneNumbers = ""
    @State var message = ""
    @State var frozen = false // try to prevent double-sending
    @ObservedObject var sendNotifier = SendNotifier()

    var body: some View {
        ZStack {
            VStack {
                Text("Phone numbers:")
                TextEditor(text: $phoneNumbers)

                Text("Message:")
                TextEditor(text: $message)

                Button("Send") {
                    sendNotifier.errors = []
                    sendNotifier.inProgress = true
                    frozen = true

                    // use asyncAfter so it will run after UI displays the progress indicator
                    DispatchQueue.main.asyncAfter(deadline: .now() + 0.01) {
                        sendNotifier.errors = send(phoneNumbers: phoneNumbers, message: message)
                            .map(ErrorInstance.init(error:))
                        sendNotifier.inProgress = false
                    }
                }
                .disabled(frozen)

                Text("Errors:")
                List(sendNotifier.errors) { errorInstance in
                    HStack {
                        if errorInstance.error as? MessageSendError == .success {
                            Image(systemName: "checkmark.circle.fill")
                                .foregroundColor(.green)
                        } else {
                            Image(systemName: "exclamationmark.circle")
                                .renderingMode(.original)
                        }

                        Text("\(errorInstance.error.localizedDescription)")
                    }
                }
            }
            .disabled(sendNotifier.inProgress)

            if sendNotifier.inProgress {
                ProgressView()

            }
        }
        .padding()
        .valueChanged(value: phoneNumbers) { _ in frozen = false }
        .valueChanged(value: message) { _ in frozen = false }
    }
}

#Preview {
    ContentView()
}
