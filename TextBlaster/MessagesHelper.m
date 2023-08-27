//
//  MessagesHelper.m
//  TextBlaster
//
//  Created by Jacob Greenfield on 8/26/23.
//

#import "MessagesHelper.h"

#import <AppKit/AppKit.h>
#import "Messages.h"

@implementation MessagesHelper {
    MessagesApplication *_messagesApp;
    NSMutableArray<NSError *> *_errors;
}

- (instancetype) initWithApplication:(SBApplication *)messagesApp {
    self = [super init];

    if (self) {
        _messagesApp = (MessagesApplication *)messagesApp;
        _messagesApp.delegate = self;
    }

    return self;
}

- (NSArray<NSError *> *)sendBlast:(nonnull NSString *)message
                   toPhoneNumbers:(nonnull NSArray<NSString *> *)phoneNumbers {
    _errors = [NSMutableArray array];
    for (NSString *phoneNumber in phoneNumbers) {
        MessagesParticipant *participant = [_messagesApp.participants objectWithName:phoneNumber];
        if (participant)
            [_messagesApp send:message to:participant];
    }
    NSMutableArray<NSError *> *result = _errors;
    return result;
}

@end


@implementation MessagesHelper (SBApplicationDelegate)
- (id)eventDidFail:(const AppleEvent *)event
         withError:(NSError *)error {
    [_errors addObject:error];
    return nil;
}
@end
