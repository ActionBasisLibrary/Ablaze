//
//  KaleidoscopeTransform.h
//  Ablaze
//
//  Created by Matt Rubin on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TouchState.h"

@interface KaleidoscopeTransform : NSObject

@property (retain) TouchState* touchState;

- (double)getCurvature;

@end
