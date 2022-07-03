#include <Arduino.h>

class IOManager{
    public:
        IOManager();
        void stateLight(int pos, boolean active);
        void getStateSensorMqtt();
        boolean verifyCanArm();
        void sensorCheckAll();
        void panelViolated();
        void pinSetConfig();
        void panelDisarm();
        void closeGarage();
        void openGarage();
        void panelArm();    
};