


// class BLEDescriptorMap {
// public:
// 	void setByUUID(const char* uuid, BLEDescriptor* pDescriptor);
// 	void setByUUID(BLEUUID uuid, BLEDescriptor* pDescriptor);
// 	void setByHandle(uint16_t handle, BLEDescriptor* pDescriptor);
// 	BLEDescriptor* getByUUID(const char* uuid);
// 	BLEDescriptor* getByUUID(BLEUUID uuid);
// 	BLEDescriptor* getByHandle(uint16_t handle);
// 	std::string	toString();
// 	void handleGATTServerEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param);
// 	BLEDescriptor* getFirst();
// 	BLEDescriptor* getNext();
// private:
// 	std::map<BLEDescriptor*, std::string> m_uuidMap;
// 	std::map<uint16_t, BLEDescriptor*> m_handleMap;
// 	std::map<BLEDescriptor*, std::string>::iterator m_iterator;
// };