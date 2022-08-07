#include <apriltags.h>
#include <pose.h>

class visionTranslator{
    private:
    //Define camera position within coordinate system
    float camHeight = 0;//cm
    float camXPos = 0;//cm
    float camYPos = 0;//cm

    //Camera parameters
    int xRes = 160;//pixels
    int yRes = 120;//pixels
    float xFOV = 1;//radians
    float yFOV = 1;//radians

    //AprilTag parameters
    float localXOffset = 0;//Distance from center of robot to tag (cm)
    float localYOffset = 0;//Distance from center of robot to tag (cm)
    float tagHeight = 0;//Height of tag off ground
    float angleOffset = 0;//Angle between tag forward direction and robot forward direction
    public:
    visionTranslator(){}
    pose getPose(AprilTagDatum tag);

};