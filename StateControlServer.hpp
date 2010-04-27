
class StateControlServer : public SimpleServer, public Process { public: Status::Status_t process(){
  DEBUGprint_HEARTBEAT(".\n");

// No packet?
  if(offsetPacket.packet == NULL)
    return Status::Status__Good;

// Need to extract C78-encoded opcode and target.
  // Don't need to sanity-check, as sourceC78 will do that.
  MAP::Data_t *data_ptr = offsetPacket.packet->get_data(offsetPacket.packet->get_header(offsetPacket.headerOffset));

// Save, or load?
  uint32_t new_raw;
  if(! offsetPacket.packet->sourceC78(new_raw, data_ptr)) return finishedWithPacket();
  bool mode_save = (new_raw > 0);

// Packet processing is complete, and the state saving/loading may take a while.
  finishedWithPacket();

  DEBUGprint_EEP("SCS:proc: md_sv %d\n", (mode_save? 1 : 0));

// Save or load
  if(mode_save)
    kernelRouter_eepromStore.saveState();
  else
    kernelRouter_eepromStore.loadState();

  return Status::Status__Good;
} } process_stateControlServer;

