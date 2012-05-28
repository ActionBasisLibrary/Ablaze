//
//  FlatViewController.h
//  Ablaze
//
//  Created by Matt Rubin on 5/15/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TouchState.h"

@class ABLWrapper;

@interface FlatViewController : UIViewController {
	TouchState* touchState;
	ABLWrapper* wrapper;
	NSTimer* timer;
}

@end
