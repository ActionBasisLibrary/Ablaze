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
#import "TrailController.h"
#import "Framerate.h"
#import "Fade.h"


@interface ViewController : GLKViewController
{
	TouchState* touchState;
	Framerate* framerate;
	
	TrailController* trails;
	Fade* fade;
}

@end
