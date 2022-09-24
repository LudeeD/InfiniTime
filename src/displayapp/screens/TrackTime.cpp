#include "displayapp/screens/TrackTime.h"
#include "displayapp/DisplayApp.h"
#include <lvgl/lvgl.h>

using namespace Pinetime::Applications::Screens;

namespace {

  TimeTrackedPretty prettifyTicks(const TickType_t timeElapsed) {
    // Centiseconds
    const int timeElapsedCentis = timeElapsed * 100 / configTICK_RATE_HZ;

    const int hundredths = (timeElapsedCentis % 100);
    const int secs = (timeElapsedCentis / 100) % 60;
    const int mins = ((timeElapsedCentis / 100) / 60) % 60;
    const int hours = ((timeElapsedCentis / 100) / 60) / 60;
    return TimeTrackedPretty {hours, mins, secs};
  }

  void eventHandler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<TrackTime*>(obj->user_data);
    screen->OnEvent(obj, event);
  }

  static const char* btnm_map[] = {"Work", "Play", "\n", "Chores", "Social", "\n", "Health", "Learn", ""};

}

TrackTime::TrackTime(DisplayApp* app) : Screen(app) {
  lv_obj_t* btnm1 = lv_btnmatrix_create(lv_scr_act(), nullptr);
  lv_obj_align(btnm1, nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  lv_obj_set_size(btnm1, 240, 180);
  lv_btnmatrix_set_map(btnm1, btnm_map);
  lv_btnmatrix_set_btn_ctrl_all(btnm1, LV_BTNMATRIX_CTRL_CHECKABLE);
  lv_obj_set_event_cb(btnm1, eventHandler);
  lv_btnmatrix_set_one_check(btnm1, true);

  lv_obj_t* title = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(title, "Total: ");
  lv_label_set_align(title, LV_LABEL_ALIGN_LEFT);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 0, -20);
}

TrackTime::~TrackTime() {
  lv_obj_clean(lv_scr_act());
}

void TrackTime::OnEvent(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_VALUE_CHANGED) {
    const char* txt = lv_btnmatrix_get_active_btn_text(obj);
    uint16_t btnId = lv_btnmatrix_get_active_btn(obj);
    printf("%s %i was pressed\n", txt, btnId);

    oldBtnPressed = 0;
  }
}
