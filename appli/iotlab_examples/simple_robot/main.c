/*
 * Simple robot
 *
 */

#include <platform.h>
#include <printf.h>
#include <string.h>
#include <math.h>

#include "lsm303dlhc.h"
#include "l3g4200d.h"
#include "event.h"

// timer alarm function
static void alarm(handler_arg_t arg);
static soft_timer_t tx_timer;
/* Period of the sensor acquisition datas */
#define ACQ_PERIOD soft_timer_ms_to_ticks(500)

//event handler
static void handle_ev(handler_arg_t arg);

#define ACC_RES (1e-3)     // The resolution is 1 mg for the +/-2g scale
#define GYR_RES (8.75e-3)  // The resolution is 8.75mdps for the +/-250dps scale


static void hardware_init()
{
    // Openlab platform init
    platform_init();
    event_init();
    soft_timer_init(); 
    // Switch off the LEDs
    leds_off(LED_0 | LED_1 | LED_2);
    // LSM303DLHC accelero sensor initialisation
    lsm303dlhc_powerdown();
    lsm303dlhc_acc_config(
			  LSM303DLHC_ACC_RATE_400HZ,	
			  LSM303DLHC_ACC_SCALE_2G,	
			  LSM303DLHC_ACC_UPDATE_ON_READ);
    // L3G4200D gyro sensor initialisation
    l3g4200d_powerdown();
    l3g4200d_gyr_config(L3G4200D_200HZ, L3G4200D_250DPS, true);
    // Initialize a openlab timer
    soft_timer_set_handler(&tx_timer, alarm, NULL);
    soft_timer_start(&tx_timer, ACQ_PERIOD, 1);
}

int main()
{
	hardware_init();
	platform_run();
	return 0;
}

static void handle_ev(handler_arg_t arg)
{
  int16_t rawacc[3];
  int16_t rawgyr[3];
  int16_t i;
  float acc[3];
  float gyr[3];

  /* Read accelerometers */ 
  lsm303dlhc_read_acc(rawacc);
  /* Read gyrometers */
  l3g4200d_read_rot_speed(rawgyr);

  /* Conversion */
  for (i=0; i < 3; i++) {
    acc[i] = rawacc[i] * ACC_RES;
    gyr[i] = rawgyr[i] * GYR_RES;
  }
  /* Peak detection */
  printf("Acc;%f;%f;%f\n", acc[0], acc[1], acc[2]);
  printf("Gyr;%f;%f;%f\n", gyr[0], gyr[1], gyr[2]);
}

static void alarm(handler_arg_t arg) {
  event_post_from_isr(EVENT_QUEUE_APPLI, handle_ev, NULL);
}