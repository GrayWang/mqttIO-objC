//
//  MQTTEncoder.h
//  MQtt Client
//
//  Created by Kyle Roche on 8/21/11.
//  Copyright (c) 2011 Demand Results. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MQTTMessage.h"

@interface MQTTEncoder : NSObject <NSStreamDelegate> {
    NSInteger       status;
    NSOutputStream* stream;
    NSRunLoop*      runLoop;
    NSString*       runLoopMode;
    NSMutableData*  buffer;
    NSInteger       byteIndex;
    id              delegate;
}

typedef enum {
    MQTTEncoderEventReady,
    MQTTEncoderEventErrorOccurred
} MQTTEncoderEvent;

typedef enum {
    MQTTEncoderStatusInitializing,
    MQTTEncoderStatusReady,
    MQTTEncoderStatusSending,
    MQTTEncoderStatusEndEncountered,
    MQTTEncoderStatusError
} MQTTEncoderStatus;

- (id)initWithStream:(NSOutputStream*)aStream
             runLoop:(NSRunLoop*)aRunLoop
         runLoopMode:(NSString*)aMode;
- (void)setDelegate:(id)aDelegate;
- (void)open;
- (void)close;
- (MQTTEncoderStatus)status;
- (void)stream:(NSStream*)sender handleEvent:(NSStreamEvent)eventCode;
- (void)encodeMessage:(MQTTMessage*)msg;

@end

@interface NSObject (MQTTEncoderDelegate)
- (void)encoder:(MQTTEncoder*)sender handleEvent:(MQTTEncoderEvent)eventCode;

@end
