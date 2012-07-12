//
//  MQTTDecoder.h
//  MQtt Client
//
//  Created by Kyle Roche on 8/21/11.
//  Copyright (c) 2011 Demand Results. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MQTTMessage.h"

@interface MQTTDecoder : NSObject <NSStreamDelegate> {
    NSInteger       status;
    NSInputStream*  stream;
    NSRunLoop*      runLoop;
    NSString*       runLoopMode;
    id              delegate;
    UInt8           header;
    UInt32          length;
    UInt32          lengthMultiplier;
    NSMutableData*  dataBuffer;
}

typedef enum {
    MQTTDecoderEventProtocolError,
    MQTTDecoderEventConnectionClosed,
    MQTTDecoderEventConnectionError
} MQTTDecoderEvent;

enum {
    MQTTDecoderStatusInitializing,
    MQTTDecoderStatusDecodingHeader,
    MQTTDecoderStatusDecodingLength,
    MQTTDecoderStatusDecodingData,
    MQTTDecoderStatusConnectionClosed,
    MQTTDecoderStatusConnectionError,
    MQTTDecoderStatusProtocolError
};

- (id)initWithStream:(NSInputStream*)aStream
             runLoop:(NSRunLoop*)aRunLoop
         runLoopMode:(NSString*)aMode;
- (void)setDelegate:(id)aDelegate;
- (void)open;
- (void)close;
- (void)stream:(NSStream*)sender handleEvent:(NSStreamEvent)eventCode;
@end

@interface NSObject (MQTTDecoderDelegate)
- (void)decoder:(MQTTDecoder*)sender newMessage:(MQTTMessage*)msg;
- (void)decoder:(MQTTDecoder*)sender handleEvent:(MQTTDecoderEvent)eventCode;

@end
