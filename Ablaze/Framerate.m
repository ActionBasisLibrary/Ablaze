//
//  Framerate.m
//  Ablaze
//
//  Created by Matt Rubin on 5/15/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "Framerate.h"

struct ticknode {
	CFTimeInterval timestamp;
	struct ticknode *next;
};

@interface Framerate () {
	struct ticknode *first;
	struct ticknode *last;
	NSUInteger count;
}
@end

@implementation Framerate

- (NSUInteger)tick
{
	// Create a new node
	struct ticknode *new = malloc( sizeof(struct ticknode) );
	new->timestamp = CFAbsoluteTimeGetCurrent();
    new->next = 0;
	
	// Append it to the list
	count++;
	if(first==0){
		first = last = new;
	} else {
		last->next = new;
		last = last->next;
	}
	
	// Remove old nodes
	while ((new->timestamp - first->timestamp)>1.0) {
		struct ticknode *temp = first;
		first = first->next;
		free(temp);
		count--;
	}
	
	return count;
}

- (void)tickAndPrint
{
	NSLog(@"     FPS: %i", [self tick]);
}

@end
