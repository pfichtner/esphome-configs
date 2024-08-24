// bt-scanner.h

#include <map>
#include <string>
#include <chrono>

class MyBLETracker {
public:
    static std::map<std::string, unsigned long> device_timestamps; // Maps device addresses to their last seen timestamp

    static unsigned long current_time() {
        return millis(); // Current time in milliseconds
    }

    static void process_ble_advertise(const std::string &address) {
        unsigned long now = current_time();
        bool is_new = false;
        bool is_stale = false;

        // Debug message to log the BLE address
        ESP_LOGD("BLE_TRACKER", "Scanned BLE device with address: %s", address.c_str());

        // Check if the device was seen before
        auto iter = device_timestamps.find(address);
        if (iter == device_timestamps.end()) {
            // Device is new
            is_new = true;
            device_timestamps[address] = now;
        } else {
            unsigned long last_seen = iter->second;
            // Update the timestamp
            device_timestamps[address] = now;

            // Check if the device is stale
            if (now - last_seen >= 60000) { // 1 minute
                is_stale = true;
            }
        }

        // Publish stale devices
        if (is_stale) {
            id(stale_device_status).publish_state(address);
        }

        // Publish new devices
        if (is_new) {
            id(new_device_status).publish_state(address);
        }

        // Clean up stale devices
        for (auto it = device_timestamps.begin(); it != device_timestamps.end(); ) {
            if (now - it->second >= 60000) { // 1 minute
                id(stale_device_status).publish_state(it->first);
                it = device_timestamps.erase(it);
            } else {
                ++it;
            }
        }
    }
};

// Initialize static member
std::map<std::string, unsigned long> MyBLETracker::device_timestamps;

