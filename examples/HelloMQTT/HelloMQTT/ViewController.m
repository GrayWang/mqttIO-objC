//
//  ViewController.m
//  HelloMQTT
//
//  Created by Elena on 31.05.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController
@synthesize scrollView;
@synthesize textMessage = _textMessage;
@synthesize connectButton;
@synthesize topicName;
@synthesize messageTable;

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    // register for keyboard notifications
    [[NSNotificationCenter defaultCenter] addObserver:self 
                                             selector:@selector(keyboardWillShow:) 
                                                 name:UIKeyboardWillShowNotification 
                                               object:self.view.window];
    // register for keyboard notifications
    [[NSNotificationCenter defaultCenter] addObserver:self 
                                             selector:@selector(keyboardWillHide:) 
                                                 name:UIKeyboardWillHideNotification 
                                               object:self.view.window];
    NSString *alphabet  = @"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXZY0123456789";
    NSMutableString *client = [NSMutableString stringWithCapacity:5];
    for (NSUInteger i = 0; i < 5; i++) {
        u_int32_t r = arc4random() % [alphabet length];
        unichar c = [alphabet characterAtIndex:r];
        [client appendFormat:@"%C", c];
    }
    clientID = client;
    messageArray = [[NSMutableArray alloc] init];
    topicArray = [[NSMutableArray alloc] init];
    CGSize scrollContentSize = self.view.frame.size;
    self.scrollView.contentSize = scrollContentSize;
}

- (void)keyboardWillHide:(NSNotification *)n
{
    NSDictionary* userInfo = [n userInfo];
    CGRect viewFrame = self.scrollView.frame;
    CGSize keyboardSize = [[userInfo objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
    viewFrame.size.height += keyboardSize.height;
    [UIView beginAnimations:@"fieldsDown" context:NULL];
    [self.scrollView setFrame:viewFrame];
    [UIView commitAnimations];
    
}

- (void)keyboardWillShow:(NSNotification *)n
{
    NSDictionary* userInfo = [n userInfo];
    CGSize keyboardSize = [[userInfo objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
    CGRect viewFrame = self.scrollView.frame;
    viewFrame.size.height -= keyboardSize.height;
    [UIView beginAnimations:@"fieldsDown" context:NULL];
    [self.scrollView setFrame:viewFrame];
    [UIView commitAnimations];
}

#pragma mark TextFieldDelegate

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return NO;
}


#pragma mark - MQtt Callback methods

- (void)session:(MQTTSession*)sender handleEvent:(MQTTSessionEvent)eventCode {
    switch (eventCode) {
        case MQTTSessionEventConnected:
            NSLog(@"connected");
            
            break;
        case MQTTSessionEventConnectionRefused:
            NSLog(@"connection refused");
            break;
        case MQTTSessionEventConnectionClosed:
            NSLog(@"connection closed");
            break;
        case MQTTSessionEventConnectionError:
            NSLog(@"connection error");
            NSLog(@"reconnecting...");
            // Forcing reconnection
            [session connectToHost:@"q.m2m.io" port:1883];
            break;
        case MQTTSessionEventProtocolError:
            NSLog(@"protocol error");
            break;
    }
}

- (void)session:(MQTTSession*)sender
     newMessage:(NSData*)data
        onTopic:(NSString*)topic {
    NSLog(@"new message, %d bytes, topic=%@", [data length], topic);
    NSString *payloadString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"data: %@ %@", payloadString,data);
    [messageArray insertObject:payloadString atIndex:0];
    [topicArray insertObject:topic atIndex:0];
    [messageTable reloadData];
    //[self newMessageReceived:payloadString Topic:topic];
    
}

#pragma mark - Table View

- (NSInteger) numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return [messageArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    UITableViewCell * cell= [tableView dequeueReusableCellWithIdentifier:@"Cell"];
    if(!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"Cell"];
    }
    cell.textLabel.text = [NSString stringWithFormat:@"%@",[messageArray objectAtIndex:indexPath.row]];
    cell.detailTextLabel.text = [NSString stringWithFormat:@"Topic: %@",[topicArray objectAtIndex:indexPath.row]];
    return cell;
}


- (void)viewDidUnload
{
    // unregister for keyboard notifications while not visible.
    [[NSNotificationCenter defaultCenter] removeObserver:self 
                                                    name:UIKeyboardWillShowNotification 
                                                  object:nil]; 
    // unregister for keyboard notifications while not visible.
    [[NSNotificationCenter defaultCenter] removeObserver:self 
                                                    name:UIKeyboardWillHideNotification 
                                                  object:nil]; 
    [self setTextMessage:nil];
    [self setConnectButton:nil];
    [self setTopicName:nil];
    [self setMessageTable:nil];
    [self setScrollView:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (IBAction)sendMessage:(id)sender {
    [_textMessage resignFirstResponder];
    NSString * string = _textMessage.text;
    
    NSData* pubData=[string dataUsingEncoding:NSUTF8StringEncoding];
    [session publishData:pubData onTopic:topicName.text];
}

- (IBAction)connect:(id)sender {
    if (!connecting) {
        NSString *alphabet  = @"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXZY0123456789";
        NSMutableString *client = [NSMutableString stringWithCapacity:5];
        for (NSUInteger i = 0; i < 5; i++) {
            u_int32_t r = arc4random() % [alphabet length];
            unichar c = [alphabet characterAtIndex:r];
            [client appendFormat:@"%C", c];
        }
        clientID = client;
        session = [[MQTTSession alloc] initWithClientId:clientID];
        [session connectToHost:@"q.m2m.io" port:1883];
        [session setDelegate:self];
        [connectButton setTitle:@"Disconnect" forState:UIControlStateNormal];
        connecting = YES;
    } else {
        session = nil;
        [connectButton setTitle:@"Connect" forState:UIControlStateNormal];
        connecting = NO;
    }
    
}

- (IBAction)subscribe:(id)sender {
    [topicName resignFirstResponder];
    [session subscribeTopic:topicName.text];
}
@end
