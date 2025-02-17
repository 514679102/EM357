#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "hal.h"
#include "bootloader-common.h"
#include "standalone-bootloader.h"
#include "bootloader-gpio.h"

//[[
////////////////////////////////////////////////////////////////////////////
// If you edit this file, be sure to also edit bootloader-gpio-ezsp-spi.c //
////////////////////////////////////////////////////////////////////////////
//]]

// Function Name: bootloadForceActivation
// Description:   Decides whether to continue launching the bootloader or vector
//                to the application instead. This decision is usually based on
//                some sort of GPIO logic.
//                (ie. a hardware trigger).
// NOTE!!!        This function is called extremely early in the boot process
//                prior to the chip being fully configured and before memory 
//                is initialized, so limited functionality is available.
//                If this function returns FALSE any state altered should
//                be returned to its reset state
// Parameters:    none
// Returns:       FALSE (0) if bootloader should not be launched.
//                (This will try to execute the application if possible.)
//                TRUE (1) if bootloader should be launched.
//
boolean bootloadForceActivation( void ) 
{ 
  #if defined(USE_BUTTON_RECOVERY)
    int32u i;
    boolean pressed;
    // this provides an example of an alternative recovery mode activation
    //  method by utilizing one of the breakout board buttons

    // Since the button may have decoupling caps, they may not be charged
    //  after a power-on and could give a false positive result.  To avoid
    //  this issue, drive the output as an output for a short time to charge
    //  them up as quickly as possible
    halGpioConfig(PORTC_PIN(6),GPIOCFG_OUT);
    GPIO_PCSET = PC6;
    for(i=0; i<100; i++)
      __no_operation();
    
    // Reconfigure as an input with pullup to read the button state
    halGpioConfig(PORTC_PIN(6),GPIOCFG_IN_PUD);
    // (IO was already set to enable the pullup above)
    // We have to delay again here so that if the button is depressed the
    //  cap has time to discharge again after being charged up 
    //  by the above delay
    for(i=0; i<500; i++)
      __no_operation();
    pressed = (GPIO_PCIN & BIT(6)) ? FALSE : TRUE;
    //restore IO to its reset state
    halGpioConfig(PORTC_PIN(6),GPIOCFG_IN);
  
    return pressed;
  #else
    return FALSE;; 
  #endif
}


//
// Function Name: bootloadGpioInit
// Description:   This function selects the chip's GPIO configuration. These 
//                settings are the minimum base configuration needed to 
//                support the bootloader functionality. These settings can 
//                be modified for custom applications as long as the base 
//                settings are preserved.
// Parameters:    none
// Returns:       none
//
void bootloadGpioInit(void) 
{
  halInternalEnableWatchDog();

  #ifdef BTL_HAS_RADIO
    if (paIsPresent()) {
      // The PHY_CONFIG token indicates that a power amplifier is present.
      // Not all power amplifiers require both TX_ACTIVE and nTX_ACTIVE.  If
      // your design does not require TX_ACTIVE or nTX_ACTIVE, you can remove
      // the GPIO configuration for the unused signal to save a bit of flash.
      
      // Configure GPIO PC5 to support TX_ACTIVE alternate output function.
      halGpioConfig(PORTC_PIN(5),GPIOCFG_OUT_ALT);
      
      // Configure GPIO PC6 to support nTX_ACTIVE alternate output function.
      halGpioConfig(PORTC_PIN(6),GPIOCFG_OUT_ALT);
    }
  #endif

  #ifndef NO_LED
    halGpioConfig(BOARD_ACTIVITY_LED, GPIOCFG_OUT);
    halClearLed(BOARD_ACTIVITY_LED);
    halGpioConfig(BOARD_HEARTBEAT_LED, GPIOCFG_OUT);
    halSetLed(BOARD_HEARTBEAT_LED);  
  #endif
  
  CONFIGURE_EXTERNAL_REGULATOR_ENABLE();  
}


// bootloadStateIndicator
//
// Called by the bootloader state macros (in bootloader-gpio.h). Used to blink
// the LED's or otherwise signal bootloader activity.
//
// Current settings: 
// Enabled: Turn on yellow activity LED on bootloader up and successful bootload
//          Turn off yellow activity LED on bootload failure
// Disabled: Blink red heartbeat LED during idle polling.
void bootloadStateIndicator(enum blState_e state) 
{
  // sample state indication using LEDs
  #ifndef NO_LED
    static int16u pollCntr = 0;
  
    switch(state) {
      case BL_ST_UP:                      // bootloader up
        halSetLed(BOARD_ACTIVITY_LED);
        break;

      case BL_ST_DOWN:                    // bootloader going down
        break;

      case BL_ST_POLLING_LOOP:            // Polling for serial or radio input in 
                                          // standalone bootloader.
        if(0 == pollCntr--) {
          halToggleLed(BOARD_HEARTBEAT_LED);
          pollCntr = 10000;
        }
        break;

      case BL_ST_DOWNLOAD_LOOP:           // processing download image
        halClearLed(BOARD_HEARTBEAT_LED);
        halToggleLed(BOARD_ACTIVITY_LED);
        break;
      
      case BL_ST_DOWNLOAD_SUCCESS: 
        halSetLed(BOARD_ACTIVITY_LED);
        break;
      
      case BL_ST_DOWNLOAD_FAILURE:
        halClearLed(BOARD_ACTIVITY_LED);
        break;

      default:
        break;
    }
  #endif
}
