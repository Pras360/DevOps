#include "MeshCom.h"
#include <Arduino.h>

void MeshCom::begin(int deviceNumber, int totalDevice){
    this->_deviceNumber = deviceNumber;
    this->_totalDevice = totalDevice;
    this->node_status = new STATUS_NODE[this->_totalDevice];
    for(int i = 0; i < this->_totalDevice; i++){
        this->node_status[i] = VEHICLE_CLEAR;
    }
}

void MeshCom::setNode(int nodeNumber, STATUS_NODE status){
    if((nodeNumber >= 0) && (nodeNumber < this->_totalDevice)){
        this->node_status[nodeNumber] = status;
    }
}

String MeshCom::getTopicPrev(){
    String _str = "t";
    int num = this->_deviceNumber - 1;

    if(num >= 0){
        _str += num;
    }else{
        _str = "";
    }
    return _str;
}

String MeshCom::getTopic(){
    String _str = "t";
    _str += (this->_deviceNumber);
    return _str;
}

String MeshCom::getTopicNext(){
    String _str = "t";
    int num = this->_deviceNumber + 1;

    if(num < this->_totalDevice){
        _str += num;
    }else{
        _str = "";
    }

    return _str;
}

bool MeshCom::getStatusLamp(){
    return this->_status_lamp;
}

void MeshCom::loop(){
    this->_status_lamp = LOW;
    if(this->node_status[this->_deviceNumber] == VEHICLE_DETECT){
        this->_status_lamp = HIGH;
    }

    if(this->_deviceNumber < this->_totalDevice){
        if(this->node_status[this->_deviceNumber + 1] == VEHICLE_DETECT){
            this->_status_lamp = HIGH;
        }
    }

    if(this->_deviceNumber > 0){
        if(this->node_status[this->_deviceNumber - 1] == VEHICLE_DETECT){
            this->_status_lamp = HIGH;
        }
    }
}