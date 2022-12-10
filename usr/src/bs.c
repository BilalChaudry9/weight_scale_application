/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *  @brief LED Button Service (LBS) sample
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "../inc/bs.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bt_bs, CONFIG_BT_LBS_LOG_LEVEL);

static bool                   notify_enabled;
static bool                   button_state;
static struct bt_bs_cb        bs_cb;

static void wslc_ccc_cfg_changed(const struct bt_gatt_attr *attr,
				  uint16_t value)
{
	notify_enabled = (value == BT_GATT_CCC_NOTIFY);
}

static ssize_t write_battery(struct bt_conn *conn,
			 const struct bt_gatt_attr *attr,
			 const void *buf,
			 uint16_t len, uint16_t offset, uint8_t flags)
{
	LOG_DBG("Attribute write, handle: %u, conn: %p", attr->handle,
		(void *)conn);

	if (len != 1U) {
		LOG_DBG("Write Battery: Incorrect data length");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (offset != 0) {
		LOG_DBG("Write Battery: Incorrect data offset");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	if (bs_cb.battery_cb) {
		uint8_t val = *((uint8_t *)buf);

		if (val > 0x64 ) {
			//bs_cb.battery_cb(val);
		} else {
			LOG_DBG("Write Battery: Incorrect value");
			return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
		}
	}

	return len;
}

#ifdef CONFIG_BT_LBS_POLL_BUTTON
static ssize_t read_button(struct bt_conn *conn,
			  const struct bt_gatt_attr *attr,
			  void *buf,
			  uint16_t len,
			  uint16_t offset)
{
	const char *value = attr->user_data;

	LOG_DBG("Attribute read, handle: %u, conn: %p", attr->handle,
		(void *)conn);

	if (bs_cb.battery_cb) {
		button_state = bs_cb.battery_cb();
		return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
					 sizeof(*value));
	}

	return 0;
}
#endif

/* LED Button Service Declaration */
BT_GATT_SERVICE_DEFINE(bs_svc,
BT_GATT_PRIMARY_SERVICE(BT_UUID_BS),
#ifdef CONFIG_BT_LBS_POLL_BUTTON
	BT_GATT_CHARACTERISTIC(BT_UUID_BATTERY_LEVEL,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, read_button, NULL,
			       &button_state),
#else
	BT_GATT_CHARACTERISTIC(BT_UUID_BATTERY_LEVEL,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, NULL, NULL, NULL),
#endif
	/*BT_GATT_CHARACTERISTIC(BT_UUID_BATTERY_LEVEL,
			       BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_WRITE,
			       NULL, write_battery, NULL),*/
	// BT_GATT_CCC(wslc_ccc_cfg_changed,
	// 	    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

int bt_bs_init(struct bt_bs_cb *callbacks)
{
	if (callbacks) {
		//ws_cb.led_cb    = callbacks->led_cb;
		bs_cb.battery_cb = callbacks->battery_cb;
	}

	return 0;
}

int bt_bs_send_button_state(bool button_state)
{
	if (!notify_enabled) {
		return -EACCES;
	}

	return bt_gatt_notify(NULL, &bs_svc.attrs[2],
			      &button_state,
			      sizeof(button_state));
}
