//
//  MessagesHelper.h
//  TextBlaster
//
//  Created by Jacob Greenfield on 8/26/23.
//

#import <ScriptingBridge/ScriptingBridge.h>

NS_ASSUME_NONNULL_BEGIN

@interface MessagesHelper : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype) initWithApplication:(SBApplication *)messagesApp;
- (NSArray<NSError *> *)sendBlast:(NSString *)message
                   toPhoneNumbers:(NSArray<NSString *> *)phoneNumbers;

@end

@interface MessagesHelper (SBApplicationDelegate) <SBApplicationDelegate>
@end

NS_ASSUME_NONNULL_END
