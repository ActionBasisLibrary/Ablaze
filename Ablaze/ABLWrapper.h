//
//  ABLWrapper.h
//  Ablaze
//
//  Created by Matt Rubin on 5/8/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ABL/ABTransform.h"
#import "ABL/ABSymbolSubclasses.h"
#import "TouchState.h"

@interface ABLWrapper : NSObject
{
	ABTransform transform;
	TouchState* touchState;
}
@property (retain) TouchState* touchState;
-(void)prepTouchState;
-(CGPoint)getMean;
@end
