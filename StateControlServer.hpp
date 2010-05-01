#include "../common/MapOS/StateControlServer.hpp"

class LocalStateControlServer : public StateControlServer { public:
  void saveState(){
    kernelRouter_eepromStore.saveState();
  }
  void loadState(){
    kernelRouter_eepromStore.loadState();
  }  
} process_stateControlServer;

