#include "ble/BLE.h"
#include "mbed.h"



/*
    Uncomment to build pairing mode hex

    Used to pair to authorise with android.
    I don't know if it is required on ios.
*/

//#define MODE_PAIR
#define DISPLAY_ENABLE
#if defined DISPLAY_ENABLE
#include "MicroBitDisplay.h"
// Remove printf... Uses too much memory
#define printf(...) {}
#else
    Serial pc(USBTX, USBRX);
#endif


#if defined MODE_PAIR 

//Pairing Mode
    #include "ble/services/DeviceInformationService.h"
    BLE ble;
    const static char DEVICE_NAME[] = "BLE Test";
    static const uint16_t uuid16_list[] = {GattService::UUID_DEVICE_INFORMATION_SERVICE};
    #if defined DISPLAY_ENABLE
    MicroBitDisplay display;
    #endif
    void passkeyDisplayCallback(Gap::Handle_t handle, const SecurityManager::Passkey_t passkey)
    {
        #if defined DISPLAY_ENABLE
        display.print("Connect over USB");
        #endif
        printf("Input passKey: ");
        for (unsigned i = 0; i < Gap::ADDR_LEN; i++)
        {
            printf("%c ", passkey[i]);
        }
        printf("\r\n");
    }

    void securitySetupCompletedCallback(Gap::Handle_t handle, SecurityManager::SecurityCompletionStatus_t status)
    {
        if (status == SecurityManager::SEC_STATUS_SUCCESS)
        {
            printf("Security success\r\n");
        }
        else
        {
            printf("Security failed\r\n");
        }
    }

    int main(){
        #if defined DISPLAY_ENABLE
        display.print(MicroBitImage("0,1,0,1,0\n0,0,1,0,1\n0,0,1,1,0\n0,0,1,0,1\n0,1,0,1,0\n"));
        #endif

        printf("Komoot BLE Micro:bit\n");
        printf("USB Serial TX: %d, RX: %d \n", USBTX, USBRX);
        printf("Initialising BLE\n");
        printf("PAIRING MODE\n");

        ble.init();
        bool enableBonding = true;
        bool requireMITM = false;

        printf("BLE Initialised\nBLE Version: %s \n", ble.getVersion());

        ble.securityManager().init(enableBonding, requireMITM, SecurityManager::IO_CAPS_DISPLAY_ONLY);
        ble.securityManager().onPasskeyDisplay(passkeyDisplayCallback);
        ble.securityManager().onSecuritySetupCompleted(securitySetupCompletedCallback);

        
        DeviceInformationService deviceInfo(ble, "ARM", "Model1", "SN1", "hw-rev1", "fw-rev1", "soft-rev1");

        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
        ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);

        ble.gap().setAdvertisingInterval(1000);

        printf("Starting to advertise\n");
        ble.gap().startAdvertising();

        while (1)
        {
            ble.waitForEvent();
        }
    }
#else
// Normal Mode
#include "ble/DiscoveredCharacteristic.h"
#include "ble/DiscoveredService.h"

#include "images.h"

DiscoveredCharacteristic navigationCharacteristic;
BLE ble;
#if defined DISPLAY_ENABLE
MicroBitDisplay display;
#endif

bool foundKomootService = false;

// Old conn params... Too fast
// static const Gap::ConnectionParams_t connParams = {
//     50,
//     100,
//     0,
//     100
// };

void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params)
{
    int8_t ptr = 0;
    const uint8_t addr_byte = params->peerAddr[0];
    if (params->type != 0)
    {
        return;
    }

    // printf("adv peerAddr[%02x %02x %02x %02x %02x %02x] rssi %d, isScanResponse %u, AdvertisementType %u, dataLen %u\r\n",
    //           params->peerAddr[5], params->peerAddr[4], params->peerAddr[3], params->peerAddr[2], params->peerAddr[1], addr_byte,
    //           params->rssi, params->isScanResponse, params->type, params->advertisingDataLen);

    // printf("adv data: ");
    // for (unsigned i = 0; i < params->advertisingDataLen; i++){
    //     printf("%02x ", params->advertisingData[i]);
    // }
    // printf("END\n");

    // I don't really care about the structure of the advertising payload. From testing, the last 4 uint8s are always 0x71c1e128 (The short UUID)
    uint32_t advertising_service = (params->advertisingData[params->advertisingDataLen - 1] << 24) + (params->advertisingData[params->advertisingDataLen - 2] << 16) + (params->advertisingData[params->advertisingDataLen - 3] << 8) + params->advertisingData[params->advertisingDataLen - 4];

    // Crashes USB lol maybe should make it scan slower...
    //printf("Found Service: Service UUID short: %u\n", advertising_service);

    if (advertising_service == 0x71c1e128){
        printf("Found Komoot Service\n");
        ble.gap().connect(params->peerAddr, BLEProtocol::AddressType::RANDOM_STATIC, NULL, NULL);
    }
}

void serviceDiscoveryCallback(const DiscoveredService *service)
{
    printf("Discovered service\r\n");
    if (service->getUUID().shortOrLong() == UUID::UUID_TYPE_SHORT)
    {
        printf("S UUID-%x attrs[%u %u]\r\n", service->getUUID().getShortUUID(), service->getStartHandle(), service->getEndHandle());
    }
    else
    {
        const uint8_t *longUUIDBytes = service->getUUID().getBaseUUID();
        for (unsigned i = 0; i < UUID::LENGTH_OF_LONG_UUID; i++)
        {
            printf("%02x", longUUIDBytes[i]);
        }
        printf(" attrs[%u %u]\r\n", service->getStartHandle(), service->getEndHandle());
    }
}

void characteristicDiscoveryCallback(const DiscoveredCharacteristic *characteristicP)
{
    printf("  C UUID-%x valueAttr[%u] props[%x]\r\n", characteristicP->getUUID().getShortUUID(), characteristicP->getValueHandle(), (uint8_t)characteristicP->getProperties().broadcast());
    if (characteristicP->getUUID().getShortUUID() == 0xd605)
    { /* !ALERT! Alter this filter to suit your device. */
        printf("Found komoot characteristic!\n");
        #if defined DISPLAY_ENABLE
            display.print((ImageData*)connected);
        #endif
        foundKomootService = true;
        navigationCharacteristic = *characteristicP;
        //navigationCharacteristic.read();
    }
}

void discoveryTerminationCallback(Gap::Handle_t connectionHandle)
{
    printf("terminated SD for handle %u\r\n", connectionHandle);
    if (!foundKomootService)
    {
        ble.gap().disconnect(connectionHandle, Gap::DisconnectionReason_t::LOCAL_HOST_TERMINATED_CONNECTION);
    }
}

void connectionCallback(const Gap::ConnectionCallbackParams_t *params)
{
    if (params->role == Gap::CENTRAL)
    {
        ble.gattClient().onServiceDiscoveryTermination(discoveryTerminationCallback);
        ble.gattClient().launchServiceDiscovery(params->handle, serviceDiscoveryCallback, characteristicDiscoveryCallback, UUID("71C1E128-D92F-4FA8-A2B2-0F171DB3436C"), UUID("503DD605-9BCB-4F6E-B235-270A57483026"));
    }
    else
    {
        ble.gap().disconnect(params->handle, Gap::DisconnectionReason_t::LOCAL_HOST_TERMINATED_CONNECTION);
    }
}

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    foundKomootService = false;
    #if defined DISPLAY_ENABLE
        display.print((ImageData*)disconnected);
    #endif
    printf("disconnected\r\n");
    ble.gap().startScan(advertisementCallback);
}

void timeoutCallback(const Gap::TimeoutSource_t source){
    printf("timeout\n");
    foundKomootService = false;
    ble.gap().startScan(advertisementCallback);
}

void onKomootData(const GattHVXCallbackParams *params){
    uint8_t direction = params->data[4];
    
    uint32_t distance = (params->data[5] << 24) + (params->data[6] << 16) + (params->data[7] << 8) + params->data[8];
    printf("Direction %d, Distance %d\n", direction, distance);
    #if defined DISPLAY_ENABLE
    MicroBitImage i;
    switch(direction){
        case 0:
            i = (ImageData*)0;
            break;
        case 1:
            i = (ImageData*)arrow_go_straight;
            break;
        case 2:
            i = (ImageData*)arrow_go_straight;
            break;
        case 3:
            i = (ImageData*)arrow_go_straight;
            break;
        case 4:
            i = (ImageData*)arrow_slight_left;
            break;
        case 5:
            i = (ImageData*)arrow_left;
            break;
        case 6:
            i = (ImageData*)arrow_sharp_left;
            break;
        case 7:
            i = (ImageData*)arrow_sharp_right;
            break;
        case 8:
            i = (ImageData*)arrow_right;
            break;
        case 9:
            i = (ImageData*)arrow_slight_right;
            break;
        case 100:
            i = (ImageData*)connected;
            break;
        default:
            i = (ImageData*)arrow_u_turn;
            break;
    }
    display.clear();
    display.print(i);
    int8_t distanceShifted = -5;
    if (distance < 100){
        distanceShifted = -4;
    }
    if (distance < 50){
        distanceShifted = -3;
    }
    if (distance < 30){
        distanceShifted = -2;
    }
    if (distance < 20){
        distanceShifted = -1;
    }
    if (distance < 10){
        distanceShifted = 0;
    }
    
    display.image.paste((ImageData*)line, distanceShifted, 4);
    #endif
}

bool anim_frame = false;
Timer anim_timer;
int last_time = 0;

int main()
{
    #if defined DISPLAY_ENABLE
        display.animate((ImageData*)loading, 0, 5, 0);
    #else
        pc.baud(115200);
    #endif
    printf("Komoot BLE Micro:bit\n");
    printf("USB Serial TX: %d, RX: %d \n", USBTX, USBRX);
    printf("Initialising BLE\n");

    ble.init();

    printf("BLE Initialised\nBLE Version: %s \n", ble.getVersion());

    ble.gap().onConnection(connectionCallback);
    ble.gap().onDisconnection(disconnectionCallback);
    ble.gap().onTimeout(timeoutCallback);

    printf("Starting scan...\n");
    
    ble.gattClient().onHVX(onKomootData);
    //ble.gap().setScanParams(100, 70, 0x0000, false);
    ble.gap().setScanParams(500,400);
    ble.gap().startScan(advertisementCallback);
    anim_timer.start();
    while (1)
    {
        #if defined DISPLAY_ENABLE
            if (anim_timer.read_ms() - last_time >= 1000){
                anim_frame = !anim_frame;
                last_time = anim_timer.read_ms();
                display.image.setPixelValue(0, 0, anim_frame && !foundKomootService);
            }
        #endif
        ble.waitForEvent();
    }
}

#endif