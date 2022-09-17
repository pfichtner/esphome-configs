#include "esphome.h"
#include <ArduinoSIP.h>

// Sip parameters
const char *SipIP       = "192.168.2.1";	// IP of the FRITZ!Box
const int   SipPORT     = 5060;			// SIP port of the FRITZ!Box
const char *SipUSER     = "<user>";	// SIP-Call username at the FRITZ!Box
const char *SipPW	= "<password>";	// SIP-Call password at the FRITZ!Box

// Dial parameters
const char *SipDIAL     = "**9";          	    // Dial number
const char *SipTEXT_1   = "Doorbell #1";	      // Dial text 1 for doorbell #1



char acSipIn[2048];
char acSipOut[2048];

Sip aSip(acSipOut, sizeof(acSipOut));

class SipSwitch: public Component, public Switch {
	public:
		void setup() override {
			//aSip.Init(SipIP, SipPORT, WiFi.localIP().toString().c_str(), SipPORT, SipUSER, SipPW, 15);
			aSip.Init(SipIP, SipPORT, "192.168.1.199", SipPORT, SipUSER, SipPW, 15);
		}
		void loop() override {
			aSip.Processing(acSipIn, sizeof(acSipIn));
		}
		void write_state(bool state) override {
			ESP_LOGD("SipSwitch", "Dialing %s as %s", SipDIAL, SipTEXT_1);
			aSip.Dial(SipDIAL, SipTEXT_1);
			publish_state(state);
		}
};

