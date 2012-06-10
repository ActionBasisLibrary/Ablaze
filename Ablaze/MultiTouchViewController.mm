//
//  MultiTouchViewController.m
//  Ablaze
//
//  Created by Matt Rubin on 4/25/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "MultiTouchViewController.h"
#import "AppDelegate.h"

#include "ABParticles.h"
#include "ABParticleShader.h"


@interface MultiTouchViewController ()
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;
@end


@implementation MultiTouchViewController

@synthesize context = _context;

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	touchState = [TouchState new];
	trails = [TrailController new];
	touchState.delegate = trails;
	framerate = [Framerate new];
	
	ABLWrapper *wrapper = [ABLWrapper new];
	wrapper.touchState = touchState;
	trails.wrapper = wrapper;
    
	fade = [Fade new];

    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
	self.preferredFramesPerSecond = 60;
	
    [self setupGL];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
	
    fade = nil;

    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
	self.context = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}


- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
    
	[fade setup];
	[trails setup];
	
	trails.projectionMatrix = GLKMatrix4MakeOrtho(0, self.view.bounds.size.width, self.view.bounds.size.height, 0, -1000, 1000);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
	[fade tearDown];
	[trails tearDown];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
	[trails update:self.timeSinceLastUpdate];
}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    // Render the fade effect
	[fade render];
	
    // Render the particle effects
	[trails render];
	
	// Measure the framerate
	//[framerate tickAndPrint];
}



# pragma mark - Touch Handling

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	[touchState handleTouches:touches];
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	[touchState handleTouches:touches];
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	[touchState handleTouches:touches];
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	[touchState handleTouches:touches];
}

@end
