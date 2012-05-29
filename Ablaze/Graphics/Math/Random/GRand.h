//
//  GRandom.h
//  GMath
//
//  Created by Caleb Jordan on 8/11/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include <stdlib.h>
#include <time.h>

#pragma GCC visibility push(default)

double randd();
float randf();

double randd(double min, double max);
float randf(float min, float max);

#pragma GCC visibility pop