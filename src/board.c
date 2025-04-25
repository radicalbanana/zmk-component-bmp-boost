/*
 * Copyright (c) 2025 sekigon-gonnoc
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/reboot.h>

#include <zmk/event_manager.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/usb.h>

// Include nRF52 specific headers for register access
#include <hal/nrf_power.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(bmp_boost, CONFIG_ZMK_LOG_LEVEL);

static int usb_status_cb(const zmk_event_t *eh) {
    struct zmk_usb_conn_state_changed *ev = as_zmk_usb_conn_state_changed(eh);
    
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }
    
    LOG_DBG("USB connection state changed: %d", ev->conn_state);
    
    // Check power source using MAINREGSTATUS register
    uint32_t mainRegStatus = NRF_POWER->MAINREGSTATUS;
    bool isVddhPowered = (mainRegStatus & POWER_MAINREGSTATUS_MAINREGSTATUS_Msk) != 0;
    
    if (isVddhPowered) {
        LOG_DBG("Power source: VDDH (MAINREGSTATUS: High voltage mode)");
    } else {
        LOG_DBG("Power source: Battery (MAINREGSTATUS: Normal voltage mode)");
        
        // Reset if state is POWERED but we're actually running on battery
        if (ev->conn_state == ZMK_USB_CONN_POWERED) {
            LOG_DBG("USB reports powered but running on VDD battery - restarting device");
            k_msleep(100); // Small delay to ensure log message is sent
            sys_reboot(SYS_REBOOT_WARM);
        }
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(bmp_boost, usb_status_cb);
ZMK_SUBSCRIPTION(bmp_boost, zmk_usb_conn_state_changed);