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

@interface ABLWrapper : NSObject
{
	ABTransform transform;
}
-(void)getMean;
@end
