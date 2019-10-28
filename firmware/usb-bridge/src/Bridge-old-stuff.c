/* =====[ WIP: Clean Command Parsing with jump tables started 2019-03-01 ]===== */
/* TODO: implement a top-level error handler */
/* to do something useful with a conditional check on UsbWriteStatusBlah(). */
    /* In the application, if you are good-paranoid, */
    /* you actually do check that the status was sent. */
    /* // */
    /* But I have yet to implement a top-level error handler */
    /* to do something useful with this information. */
    /* // */
    /* I just assume status bytes were sent */
    /* and let the UsbHost deal with the possibility */
    /* of not getting any response to its command. */
/* // */


/* I'm not using this struct's method, it's extra work for no reason. */
uint16_t BytesComing(BytesComing_s reply_size); // inline defined in .h
/* =====[ TODO: Pull useful things from here, then get rid of this function. ]===== */
// Define command functions in jump table for looking up USB commands.
/* static void SendSensorCommand( */
/*     sensor_cmd_key cmd_to_sensor,       // send this command key to the sensor */
/*     bridge_cmd_key cmd_done_by_bridge)  // and report status on this command key */
/* { */
/*     /1* Send the command. *1/ */
/*     SpiMasterWriteByte(cmd_to_sensor); */

/*     /1* TODO: add timer to timeout in case SpiSlave is never ready. *1/ */
/*     /1* The first two bytes from the SpiSlave are the remaining number of bytes */
/*      * in the response. *1/ */
/*     BytesComing_s   response_size; */
/*     SpiMasterWaitForSlaveReady(); response_size.msb = SpiMasterReadByte(); */
/*     SpiMasterWaitForSlaveReady(); response_size.lsb = SpiMasterReadByte(); */
/*     /1* The response should be two bytes. *1/ */
/*     uint16_t const nbytes_expected = 2; // 16-bit is the general case */

/*     /1* There is an unknown SPI communication error if there are not exactly 2 */
/*      * bytes coming. *1/ */
/*     /1* It could be the SpiSlave or the SpiMaster. *1/ */
/*     /1* Treat this like other BridgeCmd errors. Send back two bytes. *1/ */
/*     /1* byte 1: error code */
/*      * byte 2: command sent to SpiSlave *1/ */
/*     if ( BytesComing(response_size) != nbytes_expected ) */
/*     { UsbWriteStatusSpiBusError(cmd_to_sensor); return; } */

/*     /1* Read all of the bytes from the SpiSlave. *1/ */
/*     uint8_t rx[nbytes_expected]; */
/*     for (uint8_t index=0; index < nbytes_expected; index++) */
/*     { SpiMasterWaitForSlaveReady(); rx[index]= SpiMasterReadByte(); } */
/*     /1* There is an error if the SpiSlave is still signaling *Data Ready*. *1/ */
/*     if ( SpiSlaveShowsDataReady() ) */
/*     { UsbWriteStatusSpiBusError(cmd_to_sensor); return; } */

/*     /1* The SpiMaster has succeeded at this point, *1/ */
/*     /1* even if the SpiSlave sent an error code. *1/ */
/*     UsbWriteStatusOk(cmd_done_by_bridge); */
/*     /1* Whatever the SpiSlave sent, pass it up to the UsbHost. *1/ */
/*     uint8_t nbytes_of_data[] = {response_size.msb, response_size.lsb}; */
/*     UsbWrite(nbytes_of_data, 2); */
/*     UsbWrite(rx,nbytes_expected); */
/* } */

/* =====[ DEPRECATED ]===== */
/* void BridgeLedRed(void) */
/* { */
/*     BiColorLedRed(status_led); */
/*     UsbWriteStatusOk(BridgeLedRed_key); */
/* } */
// SpiSlave command lookup keys are defined in `lib/src/Spi.c`
/* extern sensor_cmd_key const SensorLed1Red_key; */
/* extern sensor_cmd_key const SensorLed1Green_key; */
/* void SendSensorLed1Green(void) */
/* { */
/*     SendSensorCommand( */
/*             SensorLed1Green_key,   // send this command key to the sensor */
/*         SendSensorLed1Green_key);  // and report status on this command key */
/* } */
/* void SendSensorLed1Red(void) */
/* { */
/*     SendSensorCommand( */
/*             SensorLed1Red_key,   // send this command key to the sensor */
/*         SendSensorLed1Red_key);  // and report status on this command key */ 
/* } */
/* void SendSensorLed2Green(void) */
/* { */
/*     SendSensorCommand( */
/*             SensorLed2Green_key,   // send this command key to the sensor */
/*         SendSensorLed2Green_key);  // and report status on this command key */
/* } */
/* void SendSensorLed2Red(void) */
/* { */
/*     SendSensorCommand( */
/*             SensorLed2Red_key,   // send this command key to the sensor */
/*         SendSensorLed2Red_key);  // and report status on this command key */ 
/* } */
/* void BridgeLedGreen(void) */
/* { */
/*     BiColorLedGreen(status_led); */
/*     UsbWriteStatusOk(BridgeLedGreen_key); */
/* } */
/* ---DEPRECATED--- */
/* bridge_cmd_key const BridgeLedRed_key = 0; */
/* bridge_cmd_key const BridgeLedGreen_key = 1; */
bridge_cmd_key const BridgeCfgLis_key = 0;
/* bridge_cmd_key const SendSensorLed1Red_key = 3; */
/* bridge_cmd_key const SendSensorLed1Green_key = 4; */
/* bridge_cmd_key const SendSensorLed2Red_key = 5; */
/* bridge_cmd_key const SendSensorLed2Green_key = 6; */
BridgeCmd* oldLookupBridgeCmd(bridge_cmd_key const key) {
    /* pf is an array of pointers to BridgeCmd functions */
    /* pf lives in static memory, not on the `LookupBridgeCmd` stack frame */
    static BridgeCmd* const pf[] = {
        /* BridgeLedRed, */
        /* BridgeLedGreen, */
        BridgeCfgLis,
        /* SendSensorLed1Red, */
        /* SendSensorLed1Green, */
        /* SendSensorLed2Red, */
        /* SendSensorLed2Green, */
        };
    /* Return func ptr. Prevent attempts at out-of-bounds access. */
    if (key < sizeof(pf)/sizeof(*pf))   return pf[key];
    /* Out of bounds keys return a NULL pointer. */
    else return NULL;
    /* Up to caller to check for NULL and take appropriate action. */
    /* Recommended action: tell UsbHost the command was not recognized. */
}

