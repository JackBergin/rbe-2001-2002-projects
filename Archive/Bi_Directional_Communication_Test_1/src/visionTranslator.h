#include <apriltags.h>
#include <pose.h>

class visionTranslator{
    private:
    //Define camera position within coordinate system
    float camHeight = 132.08;//cm
    float camXPos = 0;//cm
    float camYPos = 0;//cm

    //Camera parameters
    int xRes = 640;//pixels
    int yRes = 480;//pixels
    float xFOV = 1.2356;//radians
    float yFOV = 0.9704;//radians

    //AprilTag parameters
    float localXOffset = 0;//Distance from center of robot to tag (cm)
    float localYOffset = 0;//Distance from center of robot to tag (cm)
    float tagHeight = 8.255;//Height of tag off ground
    float angleOffset = 0;//Angle between tag forward direction and robot forward direction
    public:
    visionTranslator(){}
    pose getPose(AprilTagDatum tag);

};