#define DT_DRV_COMPAT zmk_behavior_oled_toggle

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>

#if IS_ENABLED(CONFIG_ZMK_DISPLAY)
#include <zmk/display.h>
#elif IS_ENABLED(CONFIG_DONGLE_DISPLAY_RAW_DASHBOARD) && DT_HAS_CHOSEN(zephyr_display)
static const struct device *const raw_oled = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
static bool raw_oled_manual_power_on = true;
#endif

LOG_MODULE_REGISTER(zmk_behavior_oled_toggle, LOG_LEVEL_WRN);

static int on_oled_toggle_pressed(struct zmk_behavior_binding *binding,
                                  struct zmk_behavior_binding_event event) {
    ARG_UNUSED(binding);
    ARG_UNUSED(event);
#if IS_ENABLED(CONFIG_ZMK_DISPLAY)
    zmk_display_toggle_manual_power();
#elif IS_ENABLED(CONFIG_DONGLE_DISPLAY_RAW_DASHBOARD) && DT_HAS_CHOSEN(zephyr_display)
    if (!device_is_ready(raw_oled)) {
        LOG_WRN("OLED device is not ready; manual toggle ignored");
        return ZMK_BEHAVIOR_OPAQUE;
    }
    raw_oled_manual_power_on = !raw_oled_manual_power_on;
    int err = raw_oled_manual_power_on ? display_blanking_off(raw_oled)
                                       : display_blanking_on(raw_oled);
    if (err) {
        LOG_WRN("OLED manual power toggle failed: %d", err);
    }
#endif
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_oled_toggle_released(struct zmk_behavior_binding *binding,
                                   struct zmk_behavior_binding_event event) {
    ARG_UNUSED(binding);
    ARG_UNUSED(event);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_oled_toggle_driver_api = {
    .binding_pressed = on_oled_toggle_pressed,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
    .binding_released = on_oled_toggle_released,
    .locality = BEHAVIOR_LOCALITY_CENTRAL,
};

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)
BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL,
                        POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_oled_toggle_driver_api);
#endif
