#include <stdint.h>

#define DEG_TO_RAD_CNST 0.0174532f
#define RAD_TO_DEG_CNST 57.2957795f

class CompSixAxis
{
public:
    CompSixAxis(float deltaTime, float tau);
    void CompStart();
    void CompUpdate();
    void CompAnglesGet(float *XAngle, float *YAngle);
    void CompAccelUpdate(float accelX, float accelY, float accelZ);
    void CompGyroUpdate(float gyroX, float gyroY, float gyroZ);
    float CompDegreesToRadians(float degrees)
    {
        return degrees * DEG_TO_RAD_CNST;
    }
    float CompRadiansToDegrees(float radians)
    {
        return radians * RAD_TO_DEG_CNST;
    }

private:
    //The time delta between updates ie. deltaT = 1/(sampling frequency)
    float deltaT;

    //Scaling factor
    float alpha;

    //Updated accelerometer readings.
    float Ax, Ay, Az;

    //Updated gyroscope readings.
    float Gx, Gy, Gz;

    //Accelerometer angles in orientation of the X and Y axes.
    float accelAngleX, accelAngleY;

    //Complementary filter angle output in regards to the X and Y axes.
    float compAngleX, compAngleY;

    //Derives the angles according to accelerometer readings
    void CompAccelCalculate();

    //Finds which quadrant of the unit circle the angle is in and formats it to a range of 0 to 2*pi
    float FormatAccelRange(float accelAngle, float accelZ);

    //Formats the Comp. Angle for faster filter convergence
    float FormatFastConverge(float compAngle, float accAngle);

    //Makes the complimentary filter angle to always lie within the range of 0 to 2*pi
    float FormatRange0to2PI(float compAngle);

    //Complimentary Filter process in its entirety
    float CompFilterProcess(float compAngle, float accelAngle, float omega);
};