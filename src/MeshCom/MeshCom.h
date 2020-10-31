#ifndef MeshCom_h
#define MeshCom_h

#include <Arduino.h>

enum STATUS_NODE{
    VEHICLE_DETECT,
    VEHICLE_CLEAR
};

class MeshCom{
private:
    STATUS_NODE *node_status;
    int _deviceNumber;
    int _totalDevice;
    bool _status_lamp;
public:
    void begin(int deviceNumber, int totalDevice);
    void setNode(int nodeNumber, STATUS_NODE status);
    String getTopicPrev();
    String getTopic();
    String getTopicNext();
    bool getStatusLamp();
    void loop();
};

#endif
