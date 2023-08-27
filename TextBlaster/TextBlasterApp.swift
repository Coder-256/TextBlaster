//
//  TextBlasterApp.swift
//  TextBlaster
//
//  Created by Jacob Greenfield on 8/26/23.
//

import SwiftUI
import Sparkle

@main
struct TextBlasterApp: App {
    private let updaterController: SPUStandardUpdaterController

    init() {
        // If you want to start the updater manually, pass false to startingUpdater and call .startUpdater() later
        // This is where you can also pass an updater delegate if you need one
        updaterController = SPUStandardUpdaterController(startingUpdater: true, updaterDelegate: nil, userDriverDelegate: nil)
    }

    var body: some Scene {
        WindowGroup {
            ContentView()
        }
        .commands {
            CommandGroup(after: .appInfo) {
                CheckForUpdatesView(updater: updaterController.updater)
            }
        }

        Settings {
            UpdaterSettingsView(updater: updaterController.updater)
        }
    }
}
