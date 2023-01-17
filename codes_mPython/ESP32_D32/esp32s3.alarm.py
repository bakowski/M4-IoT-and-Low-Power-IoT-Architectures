import time
import alarm

time_alarm = alarm.time.TimeAlarm(monotonic_time=(time.monotonic() + 600))
alarm.exit_and_deep_sleep_until_alarms(time_alarm)

