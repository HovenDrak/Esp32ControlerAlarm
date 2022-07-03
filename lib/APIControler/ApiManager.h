#include <Arduino.h>
#include <vector>

class ApiControler{
    public:
        ApiControler();
        void updateStatus(String service, String newState);
        String getStatus(String service);
};