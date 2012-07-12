//
//  MQTTSession.h
//  MQtt Client
//
//  Created by Kyle Roche on 8/21/11.
//  Copyright (c) 2011 Demand Results. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MQTTDecoder.h"
#import "MQTTEncoder.h"

typedef enum {
    MQTTSessionStatusCreated,
    MQTTSessionStatusConnecting,
    MQTTSessionStatusConnected,
    MQTTSessionStatusError
} MQTTSessionStatus;

typedef enum {
    MQTTSessionEventConnected,
    MQTTSessionEventConnectionRefused,
    MQTTSessionEventConnectionClosed,
    MQTTSessionEventConnectionError,
    MQTTSessionEventProtocolError
} MQTTSessionEvent;

@interface MQTTSession : NSObject {
    MQTTSessionStatus   status;
    NSString*           clientId;
    NSString*           userName;
    NSString*           password;
    UInt16              keepAliveInterval;
    BOOL                cleanSessionFlag;
    NSRunLoop*          runLoop;
    NSString*           runLoopMode;
    NSTimer*            timer;
    NSInteger           idleTimer;
    MQTTEncoder*        encoder;
    MQTTDecoder*        decoder;
    NSMutableArray*     queue;
    UInt16              msgId;
    id                  delegate;
}

- (id)initWithClientId:(NSString*)theClientId;
- (id)initWithClientId:(NSString*)theClientId
              userName:(NSString*)theUsername
              password:(NSString*)thePassword;
- (id)initWithClientId:(NSString*)theClientId
              userName:(NSString*)theUsername
              password:(NSString*)thePassword
             keepAlive:(UInt16)theKeepAliveInterval
          cleanSession:(BOOL)cleanSessionFlag;
- (id)initWithClientId:(NSString*)theClientId
              userName:(NSString*)theUsername
              password:(NSString*)thePassword
             keepAlive:(UInt16)theKeepAlive
          cleanSession:(BOOL)theCleanSessionFlag
               runLoop:(NSRunLoop*)theRunLoop
               forMode:(NSString*)theMode;
- (void)dealloc;
- (void)setDelegate:aDelegate;
- (void)connectToHost:(NSString*)ip port:(UInt32)port;
- (void)subscribeTopic:(NSString*)theTopic;
- (void)unsubscribeTopic:(NSString*)theTopic;
- (void)publishData:(NSData*)theData onTopic:(NSString*)theTopic;
- (void)publishJson:(id)payload onTopic:(NSString*)theTopic;
- (void)timerHandler:(NSTimer*)theTimer;
- (void)encoder:(MQTTEncoder*)sender handleEvent:(MQTTEncoderEvent) eventCode;
- (void)decoder:(MQTTDecoder*)sender handleEvent:(MQTTDecoderEvent) eventCode;
- (void)decoder:(MQTTDecoder*)sender newMessage:(MQTTMessage*) msg;

// private methods
- (void)newMessage:(MQTTMessage*)msg;
- (void)error:(MQTTSessionEvent)event;

@end

@interface NSObject (MQTTSessionDelegate)
- (void)session:(MQTTSession*)session handleEvent:(MQTTSessionEvent)eventCode;
- (void)session:(MQTTSession*)session newMessage:(NSData*)data onTopic:(NSString*)topic;

@end
