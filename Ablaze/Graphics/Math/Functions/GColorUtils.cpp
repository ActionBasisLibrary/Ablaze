//
//  GColorUtils.cpp
//  GMath
//
//  Created by Caleb Jordan on 11/29/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GColorUtils.h"

namespace gColorUtils {
    
    gVector3f getRGB(gVector3f &hsv)
    {
        return getRGB(hsv.x, hsv.y, hsv.z);
    }
    
    gVector3f getRGB(gnum hue, gnum sat, gnum val)
    {
        double M = val;
        double m = M * (1 - sat);
        
        double H = hue,
        h6 = H * 6.0f,
        h = h6 - floor(h6);
        double midUp = m + h * (M - m),
        midDown = M - h * (M - m);
        
        gVector3f rgb;
        if (h6 < 1.0 || h6 == 6.0)
            rgb.set(M, midUp, m);
        else if (h6 < 2.0)
            rgb.set(midDown, M, m);
        else if (h6 < 3.0)
            rgb.set(m, M, midUp);
        else if (h6 < 4.0)
            rgb.set(m, midDown, M);
        else if (h6 < 5.0)
            rgb.set(midUp, m, M);
        else if (h6 < 6.0)
            rgb.set(M, m, midDown);
        
        return rgb;
    }
    
    gVector3f getHSV(gVector3f &rgb)
    {
        return getHSV(rgb.x, rgb.y, rgb.z);
    }
    
    gVector3f getHSV(gnum red, gnum green, gnum blue)
    {
        return gVector3f(0.5, 0.5, 0.5);
    }
    
}