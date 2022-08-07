#ifndef __APRILTAGS_H
#define __APRILTAGS_H

#include <Arduino.h>

/*
 * AprilTagDatum is the data received from the camera 
*/
struct AprilTagDatum { uint16_t header_aa55, header_aa56, checksum, id = 0, cx, cy, w, h, rot; };

uint8_t getTagCount(void);
bool readTag(AprilTagDatum *tag);

#endif