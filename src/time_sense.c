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

static void prv_schedule_wakeup() {
if (s_interval == TS_OFF) {
    return;
  }

  wakeup_cancel_all();

  int interval = TS_INTERVAL_VALS[s_interval]; 

  time_t rawtime_now = time(NULL);
  struct tm* now = localtime(&rawtime_now); 
  now->tm_min = ((int) now->tm_min / interval) * interval;
  now->tm_sec = 0;
  
  time_t next = mktime(now) + interval * 60;

  while (wakeup_schedule(next, 0, false) < 0) {
    // if failed to schedule, try to reschedule one minute later
    next = next + 60;
  }
}

////
//// Lifecycle
////

static void prv_init(void) {
  prv_state_read();
  prv_toggle_state();
  prv_vibrate_if_needed();
  prv_schedule_wakeup();
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

