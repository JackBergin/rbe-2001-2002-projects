#include "visionTranslator.h"

pose visionTranslator::getPose(AprilTagDatum tag){
    float centeredX = tag.cx - xRes / 2;//X position relative to center (in pixels)
    float centeredY = -(tag.cy - yRes / 2);//Y position relative to center (in pixels, positive is upwards in frame)
    float fracX = centeredX / (xRes / 2);//Fractional x position, from -1 to 1
    float fracY = centeredY / (yRes / 2);//Fractional y position, from -1 to 1
    float xAngle = fracX * xFOV / 2;// Angle between directly forwards and tag (rad)
    float yAngle = fracY * yFOV / 2;//Angle between directly forwards and tag (rad)

    float relXPos = tan(xAngle) * (camHeight - tagHeight);//X position of tag relative to camera (cm)
    float relYPos = tan(yAngle) * (camHeight - tagHeight);//Y position of tag relative to camera (cm)
    float tagXPos = relXPos + camXPos;//X position of tag in coordinate space (cm)
    float tagYPos = relYPos + camYPos;//Y position of tag in coordinate space (cm)
    float tagAngle = tag.rot;//Angle of the tag (rad)
    
    float robotAngle = tagAngle + angleOffset; //Angle of the robot (from x axis) (rad)
    float xOffset = localXOffset * cos(robotAngle) - localYOffset * sin(robotAngle);//Global x distance between robot center and tag (cm)
    float yOffset = localXOffset * sin(robotAngle) + localYOffset * cos(robotAngle);//Global y distance between robot center and tag (cm)
    float xPos = tagXPos - xOffset;//Global x position
    float yPos = tagYPos - yOffset;//Global y position
    pose robotPose(xPos, yPos, robotAngle, false);//Pose object
    return robotPose;
}