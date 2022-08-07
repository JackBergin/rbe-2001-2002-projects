#pragma once
#include <math.h>
class pose
{
private:
    float xPos;  //CM
    float yPos;  //CM
    float theta; //radians
    bool status;

public:
    pose(float x, float y, float r, bool s)
    {
        xPos = x;
        yPos = y;
        theta = r;
        if (theta >= PI * 2)
        {
            theta -= PI * 2;
        }
        else if (theta < 0)
        {
            theta += PI * 2;
        }
        status = s;
    }
    pose()
    {
        xPos = 0;
        yPos = 0;
        theta = 0;
    }
    float getX()
    {
        return xPos;
    }
    float getY()
    {
        return yPos;
    }
    float getTheta()
    {
        return theta;
    }
    float getXInches()
    {
        return xPos / 2.54;
    }
    float getYInches()
    {
        return yPos / 2.54;
    }
    float getThetaDegrees()
    {
        return theta * 180 / PI;
    }
    bool getStatus()
    {
        return status;
    }

    float getDistanceTo(pose target)
    {
        float errorX = target.getX() - getX();
        float errorY = target.getY() - getY();
        float dist = sqrt(errorX * errorX + errorY * errorY);
        return dist;
    }
    float getHeadingTo(pose target)
    {
        float errorX = target.getX() - getX();
        float errorY = target.getY() - getY();
        float theta;
        if (errorX == 0)
        {
            if (errorY >= 0)
            {
                theta = PI / 2;
            }
            else
            {
                theta = PI * 3 / 2;
            }
        }
        else
        {
            theta = atan2(errorY, errorX);
        }
        if (theta < 0)
        {
            theta += PI * 2;
        }
        return theta;
    }
};