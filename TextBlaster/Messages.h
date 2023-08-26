/*
 * Messages.h
 */

#import <AppKit/AppKit.h>
#import <ScriptingBridge/ScriptingBridge.h>


@class MessagesApplication, MessagesDocument, MessagesWindow, MessagesParticipant, MessagesAccount, MessagesChat, MessagesFileTransfer;

enum MessagesSaveOptions {
	MessagesSaveOptionsYes = 'yes ' /* Save the file. */,
	MessagesSaveOptionsNo = 'no  ' /* Do not save the file. */,
	MessagesSaveOptionsAsk = 'ask ' /* Ask the user whether or not to save the file. */
};
typedef enum MessagesSaveOptions MessagesSaveOptions;

enum MessagesPrintingErrorHandling {
	MessagesPrintingErrorHandlingStandard = 'lwst' /* Standard PostScript error handling */,
	MessagesPrintingErrorHandlingDetailed = 'lwdt' /* print a detailed report of PostScript errors */
};
typedef enum MessagesPrintingErrorHandling MessagesPrintingErrorHandling;

enum MessagesServiceType {
	MessagesServiceTypeSMS = 'ssms',
	MessagesServiceTypeIMessage = 'sims'
};
typedef enum MessagesServiceType MessagesServiceType;

enum MessagesDirection {
	MessagesDirectionIncoming = 'FTic',
	MessagesDirectionOutgoing = 'FTog'
};
typedef enum MessagesDirection MessagesDirection;

enum MessagesTransferStatus {
	MessagesTransferStatusPreparing = 'FTsp',
	MessagesTransferStatusWaiting = 'FTsw',
	MessagesTransferStatusTransferring = 'FTsg',
	MessagesTransferStatusFinalizing = 'FTsz',
	MessagesTransferStatusFinished = 'FTsf',
	MessagesTransferStatusFailed = 'FTse'
};
typedef enum MessagesTransferStatus MessagesTransferStatus;

enum MessagesConnectionStatus {
	MessagesConnectionStatusDisconnecting = 'dcng',
	MessagesConnectionStatusConnected = 'conn',
	MessagesConnectionStatusConnecting = 'cong',
	MessagesConnectionStatusDisconnected = 'dcon'
};
typedef enum MessagesConnectionStatus MessagesConnectionStatus;

@protocol MessagesGenericMethods

- (void) closeSaving:(MessagesSaveOptions)saving savingIn:(NSURL *)savingIn;  // Close a document.
- (void) saveIn:(NSURL *)in_ as:(id)as;  // Save a document.
- (void) printWithProperties:(NSDictionary *)withProperties printDialog:(BOOL)printDialog;  // Print a document.
- (void) delete;  // Delete an object.
- (void) duplicateTo:(SBObject *)to withProperties:(NSDictionary *)withProperties;  // Copy an object.
- (void) moveTo:(SBObject *)to;  // Move an object to a new location.

@end



/*
 * Standard Suite
 */

// The application's top-level scripting object.
@interface MessagesApplication : SBApplication

- (SBElementArray<MessagesDocument *> *) documents;
- (SBElementArray<MessagesWindow *> *) windows;

@property (copy, readonly) NSString *name;  // The name of the application.
@property (readonly) BOOL frontmost;  // Is this the active application?
@property (copy, readonly) NSString *version;  // The version number of the application.

- (id) open:(id)x;  // Open a document.
- (void) print:(id)x withProperties:(NSDictionary *)withProperties printDialog:(BOOL)printDialog;  // Print a document.
- (void) quitSaving:(MessagesSaveOptions)saving;  // Quit the application.
- (BOOL) exists:(id)x;  // Verify that an object exists.
- (void) send:(id)x to:(id)to;  // Sends a message to a participant or to a chat.
- (void) login;  // Login to all accounts.
- (void) logout;  // Logout of all accounts.

@end

// A document.
@interface MessagesDocument : SBObject <MessagesGenericMethods>

@property (copy, readonly) NSString *name;  // Its name.
@property (readonly) BOOL modified;  // Has it been modified since the last save?
@property (copy, readonly) NSURL *file;  // Its location on disk, if it has one.


@end

// A window.
@interface MessagesWindow : SBObject <MessagesGenericMethods>

@property (copy, readonly) NSString *name;  // The title of the window.
- (NSInteger) id;  // The unique identifier of the window.
@property NSInteger index;  // The index of the window, ordered front to back.
@property NSRect bounds;  // The bounding rectangle of the window.
@property (readonly) BOOL closeable;  // Does the window have a close button?
@property (readonly) BOOL miniaturizable;  // Does the window have a minimize button?
@property BOOL miniaturized;  // Is the window minimized right now?
@property (readonly) BOOL resizable;  // Can the window be resized?
@property BOOL visible;  // Is the window visible right now?
@property (readonly) BOOL zoomable;  // Does the window have a zoom button?
@property BOOL zoomed;  // Is the window zoomed right now?
@property (copy, readonly) MessagesDocument *document;  // The document whose contents are displayed in the window.


@end



/*
 * Messages Suite
 */

// Messages application.
@interface MessagesApplication (MessagesSuite)

- (SBElementArray<MessagesParticipant *> *) participants;
- (SBElementArray<MessagesAccount *> *) accounts;
- (SBElementArray<MessagesFileTransfer *> *) fileTransfers;
- (SBElementArray<MessagesChat *> *) chats;

@end

// A participant for an account.
@interface MessagesParticipant : SBObject <MessagesGenericMethods>

- (NSString *) id;  // The participant's unique identifier. For example: 01234567-89AB-CDEF-0123-456789ABCDEF:+11234567890
@property (copy, readonly) MessagesAccount *account;  // The account for this participant.
@property (copy, readonly) NSString *name;  // The participant's name as it appears in the participant list.
@property (copy, readonly) NSString *handle;  // The participant's handle.
@property (copy, readonly) NSString *firstName;  // The first name from this participan's Contacts card, if available
@property (copy, readonly) NSString *lastName;  // The last name from this participant's Contacts card, if available
@property (copy, readonly) NSString *fullName;  // The full name from this participant's Contacts card, if available


@end

// An account that can be logged in to from this system
@interface MessagesAccount : SBObject <MessagesGenericMethods>

- (SBElementArray<MessagesChat *> *) chats;
- (SBElementArray<MessagesParticipant *> *) participants;

- (NSString *) id;  // A unique identifier for this account.
@property (copy, readonly) NSString *objectDescription;  // The name of this account as defined in Account preferences description field
@property BOOL enabled;  // Is the account enabled?
@property (readonly) MessagesConnectionStatus connectionStatus;  // The connection status for this account.
@property (readonly) MessagesServiceType serviceType;  // The type of service for this account


@end

// An SMS or iMessage chat.
@interface MessagesChat : SBObject <MessagesGenericMethods>

- (SBElementArray<MessagesParticipant *> *) participants;

- (NSString *) id;  // A guid identifier for this chat.
@property (copy, readonly) NSString *name;  // The chat's name as it appears in the chat list.
@property (copy, readonly) MessagesAccount *account;  // The account which is participating in this chat.


@end

// A file being sent or received
@interface MessagesFileTransfer : SBObject <MessagesGenericMethods>

- (NSString *) id;  // The guid for this file transfer
@property (copy, readonly) NSString *name;  // The name of this file
@property (copy, readonly) NSURL *filePath;  // The local path to this file transfer
@property (readonly) MessagesDirection direction;  // The direction in which this file is being sent
@property (copy, readonly) MessagesAccount *account;  // The account on which this file transfer is taking place
@property (copy, readonly) MessagesParticipant *participant;  // The other participatant in this file transfer
@property (readonly) NSInteger fileSize;  // The total size in bytes of the completed file transfer
@property (readonly) NSInteger fileProgress;  // The number of bytes that have been transferred
@property (readonly) MessagesTransferStatus transferStatus;  // The current status of this file transfer
@property (copy, readonly) NSDate *started;  // The date that this file transfer started


@end

