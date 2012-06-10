//
//  TrailController.m
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "TrailController.h"

@implementation TrailController
@synthesize particles, pshader;

-(void)touchAdded:(NSString*)key
{
	NSLog(@"touchAdded: %@", key);
}

-(void)touchUpdated:(NSString*)key
{
	NSLog(@"touchUpdated: %@", key);
}

-(void)touchRemoved:(NSString*)key
{
	NSLog(@"touchRemoved: %@", key);
}

@end
