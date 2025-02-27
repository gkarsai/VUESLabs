#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "libsoc_pwm.h"
#include "libsoc_debug.h"

/**
 *
 * This test is intended to be run on beaglebone white hardware,
 * however, it will work on any board with a PWM sysfs implementation.
 *
 * The PWM sysfs interface was introduced in linux 3.12, if your kernel
 * is older than this you will need to update or backport the PWM sysfs
 * patch.
 *
 * PWM sysfs Documentation: https://www.kernel.org/doc/Documentation/pwm.txt
 *
 */

#define PWM_OUTPUT_CHIP 2
#define PWM_CHIP_OUTPUT 2

/* Servo values for TG9 servos: */
/* Servo 0 degree angle pulse high time in msec */
#define SRV_0    0.45
/* Servo 180 degree angle pulse high time in msec */
#define SRV_180  2.45

/* Pulse repetition frequency in Hz */
#define FRQ 50.0f
/* Pulse period in msec */
#define PER (1.0E3/FRQ)

int main(void)
{
  int ret = EXIT_SUCCESS;

  libsoc_set_debug(1);

  pwm *pwm = libsoc_pwm_request(PWM_OUTPUT_CHIP, PWM_CHIP_OUTPUT, LS_PWM_SHARED);

  if (!pwm)
  {
    printf("Failed to get PWM\n");
    goto fail;
  }

  int period = PER * 1.0E6;	/* Period in nsec */
  libsoc_pwm_set_period(pwm, period);

  int current_period = libsoc_pwm_get_period(pwm);
  if (current_period != period){
	  printf("Failed period setting\n");
	  goto fail;
  }

  int i;
  float SM_1_duty ;	/* Servomotor 1 , connect to ePWM0A */
  int enabled;

  for(i = 0 ; i <= 180 ; i += 10) {
	/* Calculate duty cycle */
	/* Note: the 100-X duty cyle is to account for the level shifter that inverts */
	SM_1_duty = 100.0 - ((SRV_0/PER) + (i/180.0) * ((SRV_180-SRV_0)/PER))*100.0;
	printf("Angle : %d , duty : %f %%\n" ,i ,SM_1_duty);

	int duty = (SM_1_duty * 1.0E-2) * period; /* Duty cycle in nsec */

	libsoc_pwm_set_duty_cycle(pwm, duty);	/* Set up PWM duty cycle */

	int current_duty = libsoc_pwm_get_duty_cycle(pwm);
	if (current_duty != duty) {
		printf("Failed duty test\n");
	    goto fail;
	}

	libsoc_pwm_set_enabled(pwm, ENABLED);	/* Enable PWM, generate waveform */
	enabled = libsoc_pwm_get_enabled(pwm);	/* Check if enable failed */
	if (!enabled) {
		printf("Failed enabling test\n");
		ret = EXIT_FAILURE;
		goto fail;
	}

	sleep(2); /* Allow time for servo to settle and for humans to see something. */

	libsoc_pwm_set_enabled(pwm, DISABLED);	/* Disable PWM, stop generating waveform */
	enabled = libsoc_pwm_get_enabled(pwm);	/* Check if enable failed */
	if (enabled) {
		printf("Failed disabling test\n");
		ret = EXIT_FAILURE;
		goto fail;
	}

  }

  fail:

  libsoc_pwm_free(pwm);

  return ret;
}
