#define DT_DRV_COMPAT zmk_behavior_timeout_cycle

#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/activity.h>
#include <zmk/event_manager.h>
#include <zmk/events/runtime_timeout_changed.h>

enum timeout_kind {
    TIMEOUT_KIND_DISPLAY = 0,
    TIMEOUT_KIND_HALVES_SLEEP = 1,
};

static const uint32_t display_timeouts[] = {30000, 60000, 300000, 0};
static const uint32_t halves_sleep_timeouts[] = {300000, 900000, 1800000, 0};

/* Defaults match the current Hana configuration: display 60 s, halves sleep 15 min. */
static uint8_t display_timeout_index = 1;
static uint8_t halves_sleep_timeout_index = 1;

static int timeout_cycle_pressed(struct zmk_behavior_binding *binding,
                                 struct zmk_behavior_binding_event event) {
    ARG_UNUSED(event);

    if (binding->param1 == TIMEOUT_KIND_DISPLAY) {
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
        display_timeout_index = (display_timeout_index + 1) % ARRAY_SIZE(display_timeouts);
        uint32_t timeout_ms = display_timeouts[display_timeout_index];
        zmk_activity_set_idle_timeout_ms(timeout_ms);

        raise_zmk_runtime_timeout_changed((struct zmk_runtime_timeout_changed) {
            .kind = ZMK_RUNTIME_TIMEOUT_DISPLAY,
            .timeout_ms = timeout_ms,
        });
#endif
        return ZMK_BEHAVIOR_OPAQUE;
    }

    if (binding->param1 == TIMEOUT_KIND_HALVES_SLEEP) {
        halves_sleep_timeout_index =
            (halves_sleep_timeout_index + 1) % ARRAY_SIZE(halves_sleep_timeouts);
        uint32_t timeout_ms = halves_sleep_timeouts[halves_sleep_timeout_index];

#if IS_ENABLED(CONFIG_ZMK_SPLIT) && !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
#if IS_ENABLED(CONFIG_ZMK_SLEEP)
        zmk_activity_set_sleep_timeout_ms(timeout_ms);
#endif
#else
        raise_zmk_runtime_timeout_changed((struct zmk_runtime_timeout_changed) {
            .kind = ZMK_RUNTIME_TIMEOUT_HALVES_SLEEP,
            .timeout_ms = timeout_ms,
        });
#endif
        return ZMK_BEHAVIOR_OPAQUE;
    }

    return -EINVAL;
}

static int timeout_cycle_released(struct zmk_behavior_binding *binding,
                                  struct zmk_behavior_binding_event event) {
    ARG_UNUSED(binding);
    ARG_UNUSED(event);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api timeout_cycle_driver_api = {
    .binding_pressed = timeout_cycle_pressed,
    .binding_released = timeout_cycle_released,
    .locality = BEHAVIOR_LOCALITY_GLOBAL,
};

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)
BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL,
                        POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &timeout_cycle_driver_api);
#endif
