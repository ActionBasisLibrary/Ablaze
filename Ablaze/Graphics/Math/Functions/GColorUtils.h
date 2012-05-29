//
//  GColorUtils.h
//  GMath
//
//  Created by Caleb Jordan on 11/29/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#pragma once

#include "GVector3f.h"

namespace gColorUtils {
    gVector3f getRGB(gVector3f &hsv);
    gVector3f getRGB(gnum hue, gnum sat, gnum val);
    gVector3f getHSV(gVector3f &rgb);
    gVector3f getHSV(gnum red, gnum green, gnum blue);
}