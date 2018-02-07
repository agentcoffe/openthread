#include "platform-z1.h"

void __cxa_pure_virtual(void)
{
    while(1);
}

extern otError otPlatUartEnable(void);
// why?
//otInstance *sInstance;

void PlatformInit(int argc, char *argv[])
{
    /* Disable watchdog timer */
    WDTCTL = WDTPW + WDTHOLD;

    /* TODO: check how easy it would be to use the standard values */
    /* Set clock to 8MHZ with tested values */
    DCOCTL = 0x00;
    BCSCTL1 = 0x8D;
    DCOCTL = 0x80;

    otPlatUartEnable();

/*    z1AlarmInit();
    z1RandomInit();
    z1RadioInit();
*/
    __eint();

    (void)argc;
    (void)argv;
}

void PlatformProcessDrivers(otInstance *aInstance)
{
    // why?
    //sInstance = aInstance;

    // should sleep and wait for interrupts here

    z1UartProcess();
    //z1RadioProcess(aInstance);
    //z1AlarmProcess(aInstance);
}
