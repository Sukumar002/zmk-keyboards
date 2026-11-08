#define DT_DRV_COMPAT zmk_behavior_oled_brightness

#include <string.h>

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/util.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>

LOG_MODULE_REGISTER(zmk_behavior_oled_brightness, LOG_LEVEL_WRN);

#define OLED_CONTRAST_MIN 32
#define OLED_CONTRAST_MAX 255
#define OLED_CONTRAST_STEP 32
#define OLED_CONTRAST_DEFAULT 128

static uint8_t oled_contrast = OLED_CONTRAST_DEFAULT;

#if DT_HAS_CHOSEN(zephyr_display) && IS_ENABLED(CONFIG_DISPLAY)
static const struct device *const oled_device = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

static int apply_oled_contrast(void)
{
    if (!device_is_ready(oled_device)) {
        return -ENODEV;
    }

    return display_set_contrast(oled_device, oled_contrast);
}
#else
static int apply_oled_contrast(void)
{
    return -ENOTSUP;
}
#endif

#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
static const struct behavior_parameter_value_metadata oled_brightness_values[] = {
    {
        .display_name = "Decrease",
        .type = BEHAVIOR_PARAMETER_VALUE_TYPE_VALUE,
        .value = 0,
    },
    {
        .display_name = "Increase",
        .type = BEHAVIOR_PARAMETER_VALUE_TYPE_VALUE,
        .value = 1,
    },
};

static const struct behavior_parameter_metadata_set oled_brightness_param_set = {
    .param1_values = oled_brightness_values,
    .param1_values_len = ARRAY_SIZE(oled_brightness_values),
};

static const struct behavior_parameter_metadata oled_brightness_metadata = {
    .sets_len = 1,
    .sets = &oled_brightness_param_set,
};
#endif

#if IS_ENABLED(CONFIG_SETTINGS) && DT_HAS_CHOSEN(zephyr_display) && IS_ENABLED(CONFIG_DISPLAY)
static int oled_brightness_settings_set(const char *name, size_t len,
                                        settings_read_cb read_cb, void *cb_arg)
{
    if (strcmp(name, "contrast") != 0) {
        return -ENOENT;
    }
    if (len != sizeof(oled_contrast)) {
        return -EINVAL;
    }

    int rc = read_cb(cb_arg, &oled_contrast, sizeof(oled_contrast));
    if (rc < 0) {
        return rc;
    }

    if (oled_contrast < OLED_CONTRAST_MIN) {
        oled_contrast = OLED_CONTRAST_MIN;
    }
    return apply_oled_contrast();
}

SETTINGS_STATIC_HANDLER_DEFINE(oled_brightness, "oled_brightness", NULL,
                               oled_brightness_settings_set, NULL, NULL);
#endif

static int oled_brightness_pressed(struct zmk_behavior_binding *binding,
                                   struct zmk_behavior_binding_event event)
{
    ARG_UNUSED(event);

    if (binding->param1 > 1) {
        return -EINVAL;
    }

    if (binding->param1 == 1) {
        uint16_t next = (uint16_t)oled_contrast + OLED_CONTRAST_STEP;
        oled_contrast = (next > OLED_CONTRAST_MAX) ? OLED_CONTRAST_MAX : next;
    } else {
        oled_contrast = (oled_contrast <= OLED_CONTRAST_MIN + OLED_CONTRAST_STEP)
                            ? OLED_CONTRAST_MIN
                            : oled_contrast - OLED_CONTRAST_STEP;
    }

    int rc = apply_oled_contrast();
    if (rc == -ENOSYS) {
        LOG_WRN("OLED driver does not support contrast adjustment");
    } else if (rc < 0) {
        LOG_WRN("OLED contrast adjustment failed: %d", rc);
    }

#if IS_ENABLED(CONFIG_SETTINGS) && DT_HAS_CHOSEN(zephyr_display) && IS_ENABLED(CONFIG_DISPLAY)
    if (rc == 0) {
        (void)settings_save_one("oled_brightness/contrast", &oled_contrast,
                                sizeof(oled_contrast));
    }
#endif

    return ZMK_BEHAVIOR_OPAQUE;
}

static int oled_brightness_released(struct zmk_behavior_binding *binding,
                                    struct zmk_behavior_binding_event event)
{
    ARG_UNUSED(binding);
    ARG_UNUSED(event);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api oled_brightness_driver_api = {
    .binding_pressed = oled_brightness_pressed,
    .binding_released = oled_brightness_released,
    .locality = BEHAVIOR_LOCALITY_CENTRAL,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .parameter_metadata = &oled_brightness_metadata,
#endif
};

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)
BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL,
                        POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &oled_brightness_driver_api);
#endif
