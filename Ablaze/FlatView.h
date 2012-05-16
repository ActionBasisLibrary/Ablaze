//
//  FlatView.h
//  Ablaze
//
//  Created by Matt Rubin on 5/15/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TouchStateTouch.h"

@interface FlatView : UIView {
	NSArray* points;
	CGPoint middlePoint;
}
@property (retain) NSArray* points;
@property (assign) CGPoint middlePoint;


@end
