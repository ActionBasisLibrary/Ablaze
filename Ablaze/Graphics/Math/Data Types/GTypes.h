//
//  GTypes.h
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <math.h>

//#define GDOUBLE

#ifdef GDOUBLE

typedef double gnum;

#define gcos cos
#define gsin sin
#define grand randd
#define gsqrt sqrt
#define gfloor floor

#else

typedef float gnum;

#define gcos cosf
#define gsin sinf
#define grand randf
#define gsqrt sqrtf
#define gfloor floorf

#endif

#define gmin(x, y) (x < y ? x : y)
#define gmax(x, y) (x > y ? x : y)