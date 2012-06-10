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

#include "TouchTrail.h"


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define CUBE_SIZE 25.0f

// Uniform index.
enum
{
    UNIFORM_MODELVIEWPROJECTION_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    NUM_UNIFORMS
};
GLint _uniforms[NUM_UNIFORMS];

// Attribute index.
enum
{
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    NUM_ATTRIBUTES
};

GLfloat _gCubeVertexData[216] = 
{
    // Data layout for each line below is:
    // positionX, positionY, positionZ,     normalX, normalY, normalZ,
    0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,          1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    
    0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 1.0f, 0.0f,
    
    -0.5f, 0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        -1.0f, 0.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, -1.0f, 0.0f,
    
    0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f,
    
    0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 0.0f, -1.0f
};

static const GLfloat _squareVertices[] = {
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
};



@interface MultiTouchViewController () {
    GLuint _program;
    
    GLKMatrix4 _modelViewProjectionMatrix;
    GLKMatrix3 _normalMatrix;
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
	
	GLuint _vertexArray2;
    GLuint _vertexBuffer2;

}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;
@property (strong, nonatomic) GLKBaseEffect *effect2;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation MultiTouchViewController

@synthesize context = _context;
@synthesize effect = _effect;
@synthesize effect2 = _effect2;

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
    
    self.effect = [[GLKBaseEffect alloc] init];
    self.effect.light0.enabled = GL_TRUE;
    self.effect.light0.diffuseColor = GLKVector4Make(1.0f, 0.4f, 0.4f, 1.0f);
	
    self.effect2 = [[GLKBaseEffect alloc] init];
	self.effect2.constantColor = GLKVector4Make(0.0f, 0.0f, 0.0f, 0.3f);
    
    glEnable(GL_DEPTH_TEST);
    
	glEnable(GL_BLEND);
    
    glGenVertexArraysOES(1, &_vertexArray);
    glBindVertexArrayOES(_vertexArray);
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_gCubeVertexData), _gCubeVertexData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribNormal);
    glVertexAttribPointer(GLKVertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(12));
    
    glBindVertexArrayOES(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	////
	glGenVertexArraysOES(1, &_vertexArray2);
    glBindVertexArrayOES(_vertexArray2);
    
    glGenBuffers(1, &_vertexBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_squareVertices), _squareVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 12, BUFFER_OFFSET(0));
    
    glBindVertexArrayOES(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
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
    
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteVertexArraysOES(1, &_vertexArray);
    
    self.effect = nil;
    
    if (_program) {
        glDeleteProgram(_program);
        _program = 0;
    }
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
	[trails update:self.timeSinceLastUpdate];
	//NSLog(@"     FPS: %i", [framerate tick]);
}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Clear the view
	glClear(GL_DEPTH_BUFFER_BIT);

    // Render the SQUARE with GLKit
    glBindVertexArrayOES(_vertexArray2);
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    [self.effect2 prepareToDraw];
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArrayOES(0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
    // Render the cube with GLKit
    glBindVertexArrayOES(_vertexArray);
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glEnableVertexAttribArray(GLKVertexAttribNormal);
	
    [self.effect prepareToDraw];
    //glDrawArrays(GL_TRIANGLES, 0, 36);
	
    glBindVertexArrayOES(0);
    
	
    // Render the particle effects
	[trails render];
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
