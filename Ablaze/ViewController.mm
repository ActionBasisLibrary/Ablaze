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
ABLWrapper *globalWrapper;

ABParticles *particles;
ABParticleShader *pshader;
gVector3f particleStartPosition;
gVector2f currVelocity;
gVector4f particleStartColor;

void startPosFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
	vect = particleStartPosition;
}
void startVelFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
	const float speed = (1.0-2.0*randf())*100.0;
	double angle = (2.0*M_PI*randf());
	vect.x = sin(angle)*speed;
	vect.y = cos(angle)*speed;
	vect.z = 0;
}
void startColorFn(gVector4f &vect, float dt, const ABParticles::Particle *ptr) {
	vect = particleStartColor;
}

int amod = 0;
void dragAccFunction(gVector3f &vect, float dt, const ABParticles::Particle *ptr) {
    double v2 = currVelocity.length() * .001;
    double drag = 1.5 * exp(-pow(v2 + .5, -4.0)) - 1.0;
//    if (amod++ % 1000 == 0) printf("Drag %f, vel %f\n", drag, v2);
    vect.x = drag * ptr->velocity.x;
	vect.y = drag * ptr->velocity.y;
	vect.z = drag * ptr->velocity.z;
}



// Trail particle

void trailInitOverrideFn(ABParticles::Particle *ptr) {
	// Use this for setting attributes that all depend on this randomized time
	float secondsAgo = 0.1+randf()*1.0;
	// Position
	CGPoint position = [globalWrapper getPosition:secondsAgo];
	ptr->position.x = position.x-5+randf()*10;
	ptr->position.y = position.y-5+randf()*10;
	ptr->position.z = -5+randf()*10;
	
	// Color
	CGPoint velocity = [globalWrapper getVelocity:secondsAgo];
	//double linearVelocity = sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
	//double absVel = linearVelocity/1000-0.1;
	//if(absVel>1.0)absVel = 1.0;
	//if(absVel<0.0)absVel = 0.0;
	double H = atan2(velocity.y, velocity.x);
	if (H<0)
		H += 2.0*M_PI;
	H /= 2.0*M_PI;
	[[UIColor colorWithHue:H saturation:1.0 brightness:1.0 alpha:1.0] getRed:&(ptr->color.r) green:&(ptr->color.g) blue:&(ptr->color.b) alpha:&(ptr->color.a)];
/*	printf(" %f\n", H);
	if (H<0.0) {
		printf("\n\n\n!!! H<0.0 !!!\n\n\n");
	} else if (H>=2.0*M_PI) {
		printf("\n\n\n!!! H>=2.0*M_PI !!!\n\n\n");
	}
	H /= (M_PI/3.0);
	double X = 1-fabs((H/2)-floor(H/2) - 1);
	double C = 1.0;
	
	if (X < 1.0) {
		ptr->color.set(C, X, 0.0);
	} else if (X < 2.0) {
		ptr->color.set(X, C, 0.0);
	} else if (X < 3.0) {
		ptr->color.set(0.0, C, X);
	} else if (X < 4.0) {
		ptr->color.set(0.0, X, C);
	} else if (X < 5.0) {
		ptr->color.set(X, 0.0, C);
	} else if (X < 6.0) {
		ptr->color.set(C, 0.0, X);
	}
*/	
//	ptr->color.r = absVel;
//	ptr->color.g = 0.0;
//	ptr->color.b = 1.0-absVel;
//	ptr->color.a = 1.0;
	
	// Size
	ptr->size = (position.y/1000)*30.0f+5.0f;
	
	// Velocity
	double vel = (position.x/1000)*randf()*30.0+5.0;
	double angle = (2.0*M_PI*randf());
	ptr->velocity.x = vel*cos(angle);
	ptr->velocity.y = vel*sin(angle);
	ptr->velocity.z = 0;
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
    profile.accFn = dragAccFunction;
	profile.startColorFn = startColorFn;
    
    ABParticles::ProfileId pid = particles->createProfile(profile);
    
    // This creates 100 particles--only important because it makes sure all positions are 0,0,0
    particles->emitParticles(000, pid);
	
	ABParticles::Profile trailProfile;
    trailProfile.lifeSpan = 2;
    trailProfile.delay = 2;
    trailProfile.continuous = true;
	trailProfile.initOverrideFn = trailInitOverrideFn;
    
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
	CGPoint point = [wrapper getPosition:0.1];
    particleStartPosition.set(point.x, point.y, 0.0);
	
	CGPoint velocity = [wrapper getVelocity:0.1];
//    printf("Velocity @ -%f = %f %f\n", 0.1, velocity.x, velocity.y);
    currVelocity.set(velocity.x, velocity.y);
	double linearVelocity = sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
	double speedScale = CUBE_SIZE*(1.0f+sqrt(linearVelocity)/40.0f);
	
	double absVel = linearVelocity/1000;
	if(absVel>1.0)absVel = 1.0;
	if(absVel<0.0)absVel = 0.0;
	
	//NSLog(@"%f, %f = %f", velocity.x, velocity.y, absVel);
	particleStartColor.r = absVel;
	particleStartColor.g = 0.0;
	particleStartColor.b = 1.0-absVel;
	particleStartColor.a = 1.0;

	
	
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
