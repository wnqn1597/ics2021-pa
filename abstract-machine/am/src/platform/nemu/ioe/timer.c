#include <am.h>
#include <nemu.h>
#include <klib.h>
#include <riscv/riscv.h>

void __am_timer_init() {

}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint32_t sec = inl(RTC_ADDR);
  //if(sec == 0){ putch('!');}
  char buf[30];
  sprintf(buf, "%d\n", sec);
  uptime->us = 0;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
