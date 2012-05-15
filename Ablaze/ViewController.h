//
//  ViewController.h
//  Ablaze
//
//  Created by Matt Rubin on 4/25/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "TouchState.h"
#import "Framerate.h"

@class ABLWrapper;

@interface ViewController : GLKViewController
{
	ABLWrapper* wrapper;
	TouchState* touchState;
	Framerate* framerate;
}

@end
