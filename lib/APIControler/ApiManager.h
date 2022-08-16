#include <Arduino.h>
#include <vector>

class ApiControler{
    public:
        ApiControler();
        String getStatus(String service);
        void updateStatus(String service, String newState);
        void createEvent(String description, String user, String type);
};