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

#include "GRand.h"

#include "Fade.h"
#include "TouchTrail.h"


@interface MultiTouchViewController ()
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) Fade *fade;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation MultiTouchViewController

@synthesize context = _context;
@synthesize fade = _fade;

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	touchState = [TouchState new];
	trails = [TrailController new];
	touchState.delegate = trails;
	framerate = [Framerate new];

    
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
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
	self.context = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}


- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
	self.fade = [Fade new];
    
    glEnable(GL_DEPTH_TEST);
    
	glEnable(GL_BLEND);
    
	[self.fade setup];
    
    // Create program
    
    const char *vertPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"vsh"] UTF8String];
    const char *fragPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"fsh"] UTF8String];

    // Creates the particle shader--see ABParticles.vsh, .fsh
    trails.pshader = new ABParticleShader(vertPath, fragPath);

    // Initializes particle source with 2000 max capacity
    trails.particles = new ABParticles(2000);
    
    // Ignore this for now--none of it is used while debugging
    ABParticles::Profile profile;
    profile.lifeSpan = 2;
    profile.delay = 2;
    profile.continuous = true;
    
    ABParticles::ProfileId pid = trails.particles->createProfile(profile);
    
    // This creates 100 particles--only important because it makes sure all positions are 0,0,0
    trails.particles->emitParticles(000, pid);
	
	ABParticles::Profile trailProfile;
    trailProfile.lifeSpan = 2;
    trailProfile.delay = 2;
    trailProfile.continuous = true;
    
    ABParticles::ProfileId trailPid = trails.particles->createProfile(trailProfile);
	trails.particles->emitParticles(1500, trailPid);
	
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	trails.projectionMatrix = GLKMatrix4MakeOrtho(0, self.view.bounds.size.width, self.view.bounds.size.height, 0, -1000, 1000);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
	[self.fade tearDown];
    
    self.fade = nil;
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
	[trails update:self.timeSinceLastUpdate];
}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    // Render the fade effect
	[self.fade render];
	
    // Render the particle effects
	[trails render];
	
	// Measure the framerate
	[framerate tickAndPrint];
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
