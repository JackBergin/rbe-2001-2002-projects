#include <apriltags.h>
#include <Wire.h>

//use a #define here because whoever wrote the Wire library doesn't understand how to use const...
#define CAMERA_I2C_ADDRESS 0x12

uint8_t getTagCount(void)
{
    uint8_t tagCount = 0;

    delayMicroseconds(100); // Give camera time to get ready

    /* Ask for one byte, which should hold the tag count 
     * We should probably check to make sure we don't accidenally read a tag.
     * All tags start with 0xAA55, so if we get 0x55, then we should throw an error...someday...
     */
    if (Wire.requestFrom(CAMERA_I2C_ADDRESS, 1, true) == 1)
        tagCount = Wire.read();

    return tagCount;
}

bool readTag(AprilTagDatum *pTag)
{
    bool retVal = false;
    /*
     * Here we have to give the camera a little time to get ready. It needs to load
     * any tag data into its queue. 100us is hit-and-miss; 200us is reliable; we use
     * 250us here for extra "safety". Tested at "full speed", the camera takes ~50ms
     * to detect a tag, so if you're not getting reliable tag reading, try upping this
     * value.
     */

    delayMicroseconds(250); // Give camera a little time to get ready

    uint8_t size = sizeof(AprilTagDatum);
    uint8_t buffer[sizeof(AprilTagDatum)];
    
    uint8_t dataSize = Wire.requestFrom(CAMERA_I2C_ADDRESS, size, true);
    Serial.print("Expected Size: ");
    Serial.println(size);
    Serial.print("Recieved Size: ");
    Serial.println(dataSize);
    if (dataSize == sizeof(AprilTagDatum))
    {
        for (uint8_t i = 0; i < sizeof(AprilTagDatum); i++)
            buffer[i] = Wire.read();
        memcpy(pTag, buffer, sizeof(AprilTagDatum));

        retVal = true;
    }
    return retVal;
}