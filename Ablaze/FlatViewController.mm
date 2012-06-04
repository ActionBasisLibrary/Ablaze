//
//  FlatViewController.m
//  Ablaze
//
//  Created by Matt Rubin on 5/15/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "FlatViewController.h"
#import "FlatView.h"
#import "ABLWrapper.h"

@interface FlatViewController ()


- (void)updateViewTouches;
- (void)justUpdateView:(NSTimer*)theTimer;

@end

@implementation FlatViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
		NSLog(@"initWithNibName");
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	touchState = [TouchState new];
	wrapper = [ABLWrapper new];
	wrapper.touchState = touchState;

	// Do any additional setup after loading the view.
	
	timer = [NSTimer scheduledTimerWithTimeInterval:0.01 target:self selector:@selector(justUpdateView:) userInfo:nil repeats:YES];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
	[timer invalidate];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return YES;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	[touchState handleTouches:touches];
	[self updateViewTouches];
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	[touchState handleTouches:touches];
	[self updateViewTouches];
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	[touchState handleTouches:touches];
	[self updateViewTouches];
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	[touchState handleTouches:touches];
	[self updateViewTouches];
}

- (void)updateViewTouches {
	((FlatView*)self.view).points = [touchState touchPoints];
	((FlatView*)self.view).middlePoint = [wrapper getMean:0.0];
	[self.view setNeedsDisplay];
}

- (void)justUpdateView:(NSTimer*)theTimer {
	((FlatView*)self.view).middlePoint = [wrapper getMean:0.0];
	[self.view setNeedsDisplay];
}


@end
