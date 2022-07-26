#include <Arduino.h>
#include <vector>

class ApiControler{
    public:
        ApiControler();
        void updateStatus(String service, String newState);
        void createLog(String description, String user);
        String getStatus(String service);
};