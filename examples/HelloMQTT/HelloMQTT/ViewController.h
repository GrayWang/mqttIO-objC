//
//  ViewController.h
//  HelloMQTT
//
//  Created by Elena on 31.05.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MQTTSession.h"


@interface ViewController : UIViewController <UITableViewDataSource, UITableViewDelegate, UITextFieldDelegate>{
    MQTTSession *session;
    BOOL connecting;
    NSString * clientID;
    NSMutableArray * messageArray;
    NSMutableArray * topicArray;
}

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UITextField *textMessage;
@property (weak, nonatomic) IBOutlet UIButton *connectButton;
@property (weak, nonatomic) IBOutlet UITextField *topicName;
@property (weak, nonatomic) IBOutlet UITableView *messageTable;

- (IBAction)sendMessage:(id)sender;
- (IBAction)connect:(id)sender;
- (IBAction)subscribe:(id)sender;

#pragma mark - MQTT Callback methods
- (void)session:(MQTTSession*)sender handleEvent:(MQTTSessionEvent)eventCode;
- (void)session:(MQTTSession*)sender newMessage:(NSData*)data onTopic:(NSString*)topic;
@end
