//
//  ViewController.m
//  Ablaze
//
//  Created by Matt Rubin on 4/25/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "ViewController.h"
#import "AppDelegate.h"
#import "ABLWrapper.h"

#include "ABParticles.h"
#include "ABParticleShader.h"

#include "GRand.h"


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define CUBE_SIZE 25.0f

// Uniform index.
enum
{
    UNIFORM_MODELVIEWPROJECTION_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// Attribute index.
enum
{
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    NUM_ATTRIBUTES
};

GLfloat gCubeVertexData[216] = 
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

static const GLfloat squareVertices[] = {
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
};


#pragma mark Particle System Setup
ABParticles *particles;
ABParticleShader *pshader;
gVector3f particleStartPosition;

void startPosFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
	vect = particleStartPosition;
}
void startVelFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
	const float speed = 100.0;
	vect.x = (-1.0+randf()*2.0)*speed;
	vect.y = (-1.0+randf()*2.0)*speed;
	vect.z = (-1.0+randf()*2.0)*speed;
}

// Trail particle
ABLWrapper *globalWrapper;

void trailStartPosFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
	CGPoint oldPoint = [globalWrapper getPosition:0.1+randf()*1.0];
	vect.x = oldPoint.x-5+randf()*10;
	vect.y = oldPoint.y-5+randf()*10;
	vect.z = -5+randf()*10;
}
void trailStartVelFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
	vect.x = -5+randf()*10;
	vect.y = -5+randf()*10;
	vect.z = -5+randf()*10;
}
void trailAccFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
	vect.x = -5+randf()*10;
	vect.y = -5+randf()*10;
	vect.z = -5+randf()*10;
}


@interface ViewController () {
    GLuint _program;
    
    GLKMatrix4 _modelViewProjectionMatrix;
    GLKMatrix3 _normalMatrix;
    float _rotation;
    
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

@implementation ViewController

@synthesize context = _context;
@synthesize effect = _effect;
@synthesize effect2 = _effect2;

- (void)viewDidLoad
{
    [super viewDidLoad];
	touchState = [TouchState new];
	framerate = [Framerate new];
	wrapper = [ABLWrapper new];
	wrapper.touchState = touchState;

    
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(gCubeVertexData), gCubeVertexData, GL_STATIC_DRAW);
    
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 12, BUFFER_OFFSET(0));
    
    glBindVertexArrayOES(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    // Create program
    
    const char *vertPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"vsh"] UTF8String];
    const char *fragPath = [[[NSBundle mainBundle] pathForResource:@"ABParticles" ofType:@"fsh"] UTF8String];

    // Creates the particle shader--see ABParticles.vsh, .fsh
    pshader = new ABParticleShader(vertPath, fragPath);

    // Initializes particle source with 2000 max capacity
    particles = new ABParticles(2000);
    
    // Ignore this for now--none of it is used while debugging
    ABParticles::Profile profile;
    profile.lifeSpan = 2;
    profile.delay = 2;
    profile.continuous = true;
	profile.startVelFn = startVelFunction;
	profile.startPosFn = startPosFunction;
    
    ABParticles::ProfileId pid = particles->createProfile(profile);
    
    // This creates 100 particles--only important because it makes sure all positions are 0,0,0
    particles->emitParticles(500, pid);
	
	ABParticles::Profile trailProfile;
    trailProfile.lifeSpan = 2;
    trailProfile.delay = 2;
    trailProfile.continuous = true;
	trailProfile.startVelFn = trailStartVelFunction;
	trailProfile.startPosFn = trailStartPosFunction;
	trailProfile.accFn = trailAccFunction;
    
    ABParticles::ProfileId trailPid = particles->createProfile(trailProfile);
	particles->emitParticles(1500, trailPid);
	
	globalWrapper = wrapper;
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	CGPoint point = [wrapper getPosition:0.0];
	particleStartPosition.x = point.x;
	particleStartPosition.y = point.y;
	
	CGPoint velocity = [wrapper getVelocity:0.0];
	double linearVelocity = sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
	double speedScale = CUBE_SIZE*(1.0f+sqrt(linearVelocity)/20.0f);
	
	
    self.effect.transform.projectionMatrix = GLKMatrix4MakeOrtho(0, self.view.bounds.size.width, self.view.bounds.size.height, 0, -1000, 1000);
    
    GLKMatrix4 baseModelViewMatrix = GLKMatrix4MakeRotation(3.0f*_rotation, 0.0f, 0.0f, 1.0f);
	baseModelViewMatrix = GLKMatrix4Translate(baseModelViewMatrix, 1.0f, 0.0f, 0.0f);
    
    // Compute the model view matrix for the object rendered with GLKit
    GLKMatrix4 modelViewMatrix = GLKMatrix4MakeTranslation(point.x, point.y, 0.0f);
	modelViewMatrix = GLKMatrix4Scale(modelViewMatrix, speedScale, speedScale, speedScale);
    modelViewMatrix = GLKMatrix4Rotate(modelViewMatrix, _rotation, 1.0f, 1.0f, 1.0f);
    modelViewMatrix = GLKMatrix4Multiply(modelViewMatrix, baseModelViewMatrix);
    
    self.effect.transform.modelviewMatrix = modelViewMatrix;
	
    _rotation += self.timeSinceLastUpdate * 1.5f;
	
	particles->advanceParticlesBySeconds(self.timeSinceLastUpdate);
	
	NSLog(@"     FPS: %i", [framerate tick]);
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
    glDrawArrays(GL_TRIANGLES, 0, 36);
	
    glBindVertexArrayOES(0);
    
	
    // Render the particle effects
	
    pshader->engage(); // Sets current program to use
    pshader->setTransform(((GLKMatrix4)GLKMatrix4Identity).m,
						  self.effect.transform.projectionMatrix.m); // Sets the modelview and projection uniforms
	
    particles->engage(); // Sets the vertex attribute pointers
    particles->renderParticles(); // Draw the particles
    particles->disengage(); // Turns off used vertex attribute arrays
	
    pshader->disengage();
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
