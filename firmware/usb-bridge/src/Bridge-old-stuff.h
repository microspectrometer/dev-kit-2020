/* =====[ WIP: robust UsbRead functionality started 2019-03-01 ]===== */
/* I'm not using this struct and it's method, it's extra work for no reason. */
/* Every Sensor reply starts with two bytes indicating how many more bytes are coming. */
typedef struct {
    uint8_t msb;
    uint8_t lsb;
} BytesComing_s;
inline uint16_t BytesComing(BytesComing_s reply_size)
{
    return reply_size.msb<<8 | reply_size.lsb;
}

/* =====[ DEPRECATED ]===== */
/* BridgeCmd BridgeLedRed; */
/* BridgeCmd BridgeLedGreen; */
/* BridgeCmd BridgeCfgLis; */
/* BridgeCmd SendSensorLed1Red; */
/* BridgeCmd SendSensorLed1Green; */
/* BridgeCmd SendSensorLed2Red; */
/* BridgeCmd SendSensorLed2Green; */
/* =====[ DEPRECATED ]===== */
/* bridge_cmd_key const BridgeLedRed_key; */
/* bridge_cmd_key const BridgeLedGreen_key; */
/* bridge_cmd_key const SendSensorLed1Red_key; */
/* bridge_cmd_key const SendSensorLed1Green_key; */
/* bridge_cmd_key const SendSensorLed2Red_key; */
/* bridge_cmd_key const SendSensorLed2Green_key; */
