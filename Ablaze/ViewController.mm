//
//  ViewController.m
//  Ablaze
//
//  Created by Matt Rubin on 4/25/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "ViewController.h"
#import "AppDelegate.h"

#include "ABParticles.h"
#include "ABParticleShader.h"

enum TrailType {
    FIRST_TRAIL,
    FOUNTAIN,
    KALEIDOSCOPE,
    CHAOS
};

static const TrailType CURRENT_TRAIL = FIRST_TRAIL;

@interface ViewController ()
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;
@end


@implementation ViewController

@synthesize context = _context;

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	touchState = [TouchState new];
	framerate = [Framerate new];
	fade = [Fade new];
    
	NSNumber *tType = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"TrailType"];
    // Now create the trail, depending on the switch
    switch (tType?[tType intValue]:CURRENT_TRAIL) {
        case FIRST_TRAIL:
        {
			ABLWrapper *wrapper = [ABLWrapper new];
			wrapper.touchState = touchState;
            trails = [TrailController new];
            ((TrailController*)trails).wrapper = wrapper;
        }
            break;
        case FOUNTAIN:
        {
            trails = [FountainController new];
            ((FountainController*)trails).touchState = touchState;
        }
            break;
        case KALEIDOSCOPE:
        {
            trails = [Kaleidoscope new];
            ((Kaleidoscope*)trails).touchState = touchState;
			
			fade.color = GLKVector4Make(1.0, 1.0, 1.0, 0.01);
        }
            break;
        case CHAOS:
        {
            trails = [ChaosController new];
            ((ChaosController*)trails).touchState = touchState;
        }
            break;
        default:
            NSLog(@"Current Trail set to impossible value");
            break;
    }
	
    
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
	trails.projectionMatrix = GLKMatrix4MakeOrtho(0, self.view.bounds.size.width, self.view.bounds.size.height, 0, -1000, 1000);
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
