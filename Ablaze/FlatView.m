//
//  FlatView.m
//  Ablaze
//
//  Created by Matt Rubin on 5/15/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "FlatView.h"

#define TOUCH_RADIUS 40

@implementation FlatView
@synthesize points;
@synthesize middlePoint;


- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
	CGContextRef context = UIGraphicsGetCurrentContext();
	UIColor *redColor = [UIColor colorWithRed:1.0 green:0.0 blue:0.0 alpha:1.0];
	CGContextSetFillColorWithColor(context, redColor.CGColor);
	
	CGContextFillEllipseInRect(context, CGRectMake(middlePoint.x-TOUCH_RADIUS, middlePoint.y-TOUCH_RADIUS, 2*TOUCH_RADIUS, 2*TOUCH_RADIUS));
	
	UIColor *blueColor = [UIColor colorWithRed:0.0 green:0.0 blue:1.0 alpha:1.0];
	CGContextSetFillColorWithColor(context, blueColor.CGColor);
	
	NSEnumerator *e = [points objectEnumerator];
	NSValue *value;
	while ((value = (NSValue*)[e nextObject])) {
		CGPoint point = [value CGPointValue];
		CGContextFillEllipseInRect(context, CGRectMake(point.x-TOUCH_RADIUS, point.y-TOUCH_RADIUS, 2*TOUCH_RADIUS, 2*TOUCH_RADIUS));
	}
}


@end
