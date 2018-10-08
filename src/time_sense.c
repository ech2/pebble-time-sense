#include <pebble.h>

////
//// State
////

typedef enum {
  TS_OFF = 0,
  TS_EVERY_HOUR,
  TS_EVERY_30_MINS,
  TS_EVERY_15_MINS,
  TS_EVERY_10_MINS,
  TS_EVERY_5_MINS
} TS_Interval;

static const int TS_INTERVAL_VALS[] = { -1, 60, 30, 15, 10, 5 };

static const char* TS_INTERVAL_NAMES[] = {
  "Off",
  "Every hour",
  "Every 30 mins",
  "Every 15 mins",
  "Every 10 mins",
  "Every 5 mins"
};

static TS_Interval s_interval = TS_OFF;

const uint32_t TS_STATE_COUNT = 6;
const uint32_t TS_STATE_STORE_KEY = 10110; 

//// Persistence

void prv_state_read() {
  if (persist_exists(TS_STATE_STORE_KEY)) {
    s_interval = persist_read_int(TS_STATE_STORE_KEY);
  } else {
    s_interval = TS_OFF;
  }
}

void prv_state_write() {
  persist_write_int(TS_STATE_STORE_KEY, s_interval);
}

static void prv_toggle_state() {
  if(launch_reason() == APP_LAUNCH_USER || launch_reason() == APP_LAUNCH_QUICK_LAUNCH) {
    s_interval = (s_interval + 1) % TS_STATE_COUNT;
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "Current interval: %s", TS_INTERVAL_NAMES[s_interval]);
}

////
//// UI
////

static void prv_update_app_glance(AppGlanceReloadSession *session, size_t limit, void *context) {
  if (limit < 1) {
    return;
  }

  const AppGlanceSlice entry = (AppGlanceSlice) {
    .layout = {
      .subtitle_template_string = TS_INTERVAL_NAMES[s_interval]
    },
    .expiration_time = time(NULL) + 3600
  };

  const AppGlanceResult result = app_glance_add_slice(session, entry);
}

////
//// Vibration
////

static void prv_vibrate_if_needed() {
  if (launch_reason() == APP_LAUNCH_WAKEUP) {
    vibes_double_pulse();
  }
}

////
//// Wakeup
//// 

static void prv_setup_wakeup() {
if (s_interval == TS_OFF) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Will not reschedule");
    return;
  }

  wakeup_cancel_all();

  time_t rawtime_now = time(NULL);
  struct tm* now = localtime(&rawtime_now);
  int now_hour = now->tm_hour;

  int interval = TS_INTERVAL_VALS[s_interval]; 
  int next_min = (now->tm_min / interval + 1) * interval;

  while (true) {
    now->tm_hour = (now_hour + (next_min / 60)) % 24; 
    now->tm_min = next_min % 60;
    now->tm_sec = 0;

    if (wakeup_schedule(mktime(now), 0, false) >= 0) {
      APP_LOG(APP_LOG_LEVEL_INFO, "Next vibration: %d:%d", now->tm_hour, now->tm_min);
      break;
    } else {
      // try to reschedule one minute later
      next_min = next_min + 1;
      APP_LOG(APP_LOG_LEVEL_INFO, "Rescheduling. Next min: %d", next_min);
    }
  }
}

////
//// Lifecycle
////

static void prv_init(void) {
  prv_state_read();
  prv_toggle_state();
  prv_vibrate_if_needed();
  prv_setup_wakeup();
}

static void prv_deinit(void) {
  prv_state_write();
  app_glance_reload(prv_update_app_glance, NULL);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
