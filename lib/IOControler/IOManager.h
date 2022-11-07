#include <Arduino.h>

class IOManager{
    public:
        IOManager();
        void stateLight(int pos, boolean active);
        void setorBypass(int setor, String user);
        void setPanelVioled(boolean violed);
        boolean verifyPanelVioled();
        void verifySensorsBypass();
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