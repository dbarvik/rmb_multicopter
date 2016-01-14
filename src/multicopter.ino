#define SERIAL_PORT_SPEED 115200

#include "schedule.h"
#include "imu.h"
#include "debugger.h"
#include "flight_controller.h"
#include "serial_commands.h"
#include "remote_control.h"

#define RESTART_ADDR       0xE000ED0C
#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

void prog_reset() {
  WRITE_RESTART(0x5FA0004);
}

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);
  Serial2.begin(SERIAL_PORT_SPEED);
  imu_init();
  rc_init();
  fc_init();
  debugger_leds_init();

  pinMode(0, INPUT);
  *portConfigRegister(0) |= PORT_PCR_PE; //pull enable
  *portConfigRegister(0) &= ~PORT_PCR_PS; //pull down
  attachInterrupt(0, prog_reset, RISING);
}

extern "C" {    // another way
  int _getpid(void) {
    return 1;
  }

  int _kill(int pid, int sig) {
    return -1;
  }

  void run_loop();
};


void loop() {
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);

//digitalWrite(14, HIGH);
  run_loop();

  //if (schedule(TASK_1000HZ)) {
  //  imu_read_raw_values();

  //  if (schedule(TASK_50HZ)) {
  //    rc_read_values();
  //    serial_commands_process();
  //  }

  //  if (schedule(TASK_500HZ)) {
  //    imu_process_values();
  //    fc_process();
  //  }

  //  if (schedule(TASK_2HZ)) {
  //    debugger_leds();
  //  }

  //  schedule_end();
  //}

  //debugger_print();
}
