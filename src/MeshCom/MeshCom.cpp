#include "MeshCom.h"
#include <Arduino.h>

void MeshCom::begin(int deviceNumber, int totalDevice){
    this->_deviceNumber = deviceNumber;
    this->_totalDevice = totalDevice;
    this->node_status = new STATUS_NODE[this->_totalDevice];
    this->device_status = new STATUS_DEVICE[this->_totalDevice];
    for(int i = 0; i < this->_totalDevice; i++){
        this->node_status[i] = VEHICLE_CLEAR;
        this->device_status[i] = DEVICE_OFF;
    }
}

void MeshCom::setNode(int nodeNumber, STATUS_NODE status){
    if((nodeNumber >= 0) && (nodeNumber < this->_totalDevice)){
        this->node_status[nodeNumber] = status;
    }
}

void MeshCom::setNode(int nodeNumber, STATUS_DEVICE status){
    if((nodeNumber >= 0) && (nodeNumber < this->_totalDevice)){
        this->device_status[nodeNumber] = status;
    }
}

String MeshCom::getTopicPrev(String prefix){
    String _str = "";
    int num = this->_deviceNumber - 1;
    for(int i = num; i >= 0; i--){
        if(i >= 0){
            if(device_status[i] == DEVICE_ON){
                _str += prefix;
                _str += i;
                break;
            }
        }else{
            _str = "";
        }
    }
    return _str;
}

String MeshCom::getTopic(String prefix){
    String _str = prefix;
    _str += (this->_deviceNumber);
    return _str;
}

String MeshCom::getTopicNext(String prefix){
    String _str = "";
    int num = this->_deviceNumber + 1;
    for(int i = num; i < this->_totalDevice; i++){
        if(i < this->_totalDevice){
            if(device_status[i] == DEVICE_ON){
                _str += prefix;
                _str += i;
                break;
            }
        }else{
            _str = "";
        }
    }

    return _str;
}

int MeshCom::getDeviceNumber(){
    return this->_deviceNumber;
}

int MeshCom::getDeviceNumberPrev(){
    int device_number;
    int num = this->_deviceNumber - 1;
    for(int i = num; i >= 0; i--){
        if(i >= 0){
            if(device_status[i] == DEVICE_ON){
                device_number = i;
                break;
            }
        }else{
            device_number = -1;
        }
    }
    return device_number;
}

int MeshCom::getDeviceNumberNext(){
    int device_number;
    int num = this->_deviceNumber + 1;
    for(int i = num; i < this->_totalDevice; i++){
        if(i < this->_totalDevice){
            if(device_status[i] == DEVICE_ON){
                device_number = i;
                break;
            }
        }else{
            device_number = -1;
        }
    }

    return device_number;
}

bool MeshCom::getStatusLamp(){
    return this->_status_lamp;
}

void MeshCom::loop(){
    this->_status_lamp = LOW;
    if(this->node_status[this->_deviceNumber] == VEHICLE_DETECT){
        this->_status_lamp = HIGH;
    }

    if(this->getDeviceNumberNext() < this->_totalDevice){
        if(this->node_status[this->getDeviceNumberNext()] == VEHICLE_DETECT){
            this->_status_lamp = HIGH;
        }
    }

    if(this->getDeviceNumberPrev() >= 0){
        if(this->node_status[this->getDeviceNumberPrev()] == VEHICLE_DETECT){
            this->_status_lamp = HIGH;
        }
    }
}