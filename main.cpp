#include "../megalm_host.h"
#include "../MegaLM.ino"
int main(){ setup(); for(;;){ if(!Serial.available()) break; loop(); } return 0; }
