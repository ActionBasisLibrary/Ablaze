//
//  Fade.m
//  Ablaze
//
//  Created by Matt Rubin on 6/9/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import "Fade.h"

static const GLfloat _squareVertices[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
};

@interface Fade ()
{
    GLuint _vertexArray;
    GLuint _vertexBuffer;
}
@property (strong, nonatomic) GLKBaseEffect *effect;
@end

@implementation Fade
@synthesize effect = _effect;

- (id)init
{
	if(self = [super init]){
		self.effect = [[GLKBaseEffect alloc] init];
		self.effect.constantColor = GLKVector4Make(0.0f, 0.0f, 0.0f, 0.3f);
	}
	return self;
}

- (GLKVector4)color
{
	return self.effect.constantColor;
}
- (void)setColor:(GLKVector4)color
{
	self.effect.constantColor = color;
}


# pragma mark Drawable Protocol

- (void)setup
{
	glGenVertexArraysOES(1, &_vertexArray);
    glBindVertexArrayOES(_vertexArray);
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_squareVertices), _squareVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 12, 0);
    
    glBindVertexArrayOES(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

- (void)tearDown
{
	glDeleteBuffers(1, &_vertexBuffer);
    glDeleteVertexArraysOES(1, &_vertexArray);
}

- (void)update:(NSTimeInterval)timeSinceLastUpdate
{}

- (void)render
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_DEPTH_BUFFER_BIT);

	glBindVertexArrayOES(_vertexArray);
    glEnableVertexAttribArray(GLKVertexAttribPosition);
	
    [self.effect prepareToDraw];
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableVertexAttribArray(GLKVertexAttribPosition);
    glBindVertexArrayOES(0);
	
	glClear(GL_DEPTH_BUFFER_BIT);
}

@end
