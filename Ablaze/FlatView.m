//
//  FlatView.m
//  Ablaze
//
//  Created by Matt Rubin on 5/15/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "FlatView.h"

@implementation FlatView

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
	UIColor *redColor = [[UIColor alloc] initWithRed:1.0 green:0.0 blue:0.0 alpha:1.0];
	CGContextSetFillColorWithColor(context, redColor.CGColor);
	CGContextFillRect(context, self.bounds);
}


@end
