#ifndef MeshCom_h
#define MeshCom_h

#include <Arduino.h>

enum STATUS_NODE{
    VEHICLE_DETECT,
    VEHICLE_CLEAR
};

enum STATUS_DEVICE{
    DEVICE_ON,
    DEVICE_OFF
};

class MeshCom{
private:
    STATUS_NODE *node_status;
    STATUS_DEVICE *device_status;
    int _deviceNumber;
    int _totalDevice;
    bool _status_lamp;
public:
    void begin(int deviceNumber, int totalDevice);
    void setNode(int nodeNumber, STATUS_NODE status);
    void setNode(int nodeNumber, STATUS_DEVICE status);
    String getTopicPrev(String prefix);
    String getTopic(String prefix);
    String getTopicNext(String prefix);
    int getDeviceNumber();
    int getDeviceNumberPrev();
    int getDeviceNumberNext();
    bool getStatusLamp();
    void loop();
};

#endif
