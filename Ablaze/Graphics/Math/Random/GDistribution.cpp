//
//  GDistribution.cpp
//  GMath
//
//  Created by Caleb Jordan on 8/12/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GDistribution.h"

// 3D

gVector3f gDistribution::gCube()
{
    return gVector3f(grand(), grand(), grand());
}

gVector3f gDistribution::gSphere()
{
    gnum th = grand() * M_PI * 2.0;
    gnum ph = grand() * M_PI;
    gnum ra = 0.5 * grand();
    return gVector3f(ra * gcos(th) * gsin(ph) + 0.5, ra * gsin(th) * gsin(ph) + 0.5, ra * gcos(ph) + 0.5);
}

gVector3f gDistribution::gShell()
{
    gnum th = grand() * M_PI * 2.0;
    gnum ph = grand() * M_PI;
    return gVector3f(0.5 * gcos(th) * gsin(ph) + 0.5, 0.5 * gsin(th) * gsin(ph) + 0.5, 0.5 * gcos(ph) + 0.5);
}

// 2D

gVector2f gDistribution::gSquare()
{
    return gVector2f(grand(), grand());
}

gVector2f gDistribution::gDisk()
{
    gnum th = grand() * M_PI * 2.0;
    gnum ra = 0.5 * grand();
    return gVector2f(ra * gcos(th) + 0.5, ra * gsin(th) + 0.5);
}

gVector2f gDistribution::gCircle()
{
    gnum th = grand() * M_PI * 2.0;
    return gVector2f(0.5 * gcos(th) + 0.5, 0.5 * gsin(th) + 0.5);
}

// 3D

gnum gDistribution::gDefault()
{
    return grand();
}