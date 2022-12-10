/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef BT_BS_H_
#define BT_BS_H_

/**@file
 * @defgroup bt_lbs LED Button Service API
 * @{
 * @brief API for the LED Button Service (LBS).
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/types.h>

/** @def BT_UUID_BS_VAL
 *  @brief Battery Service UUID value
 */
#define BT_UUID_BS_VAL 0x180f
/** @def BT_UUID_BS
 *  @brief Battery Service
 */
#define BT_UUID_BS \
	BT_UUID_DECLARE_16(BT_UUID_BS_VAL)

/** @def BT_UUID_BATTERY_LEVEL_VAL
 *  @brief Battery Level Characteristic UUID value
 */
#define BT_UUID_BATTERY_LEVEL_VAL 0x2a19
/** @def BT_UUID_BATTERY_LEVEL
 *  @brief Battery Level Characteristic
 */
#define BT_UUID_BATTERY_LEVEL \
	BT_UUID_DECLARE_16(BT_UUID_BATTERY_LEVEL_VAL)


/** @brief Callback type for when an LED state change is received. */
//typedef void (*led_cb_t)(const bool led_state);

/** @brief Callback type for when the button state is pulled. */
typedef uint8_t (*battery_cb_t)(void);

/** @brief Callback struct used by the WS Service. */
struct bt_bs_cb {
	/** LED state change callback. */
	//led_cb_t    led_cb;
	/** Battery read callback. */
	battery_cb_t battery_cb;
};

/** @brief Initialize the LBS Service.
 *
 * This function registers a GATT service with two characteristics: Button
 * and LED.
 * Send notifications for the Button Characteristic to let connected peers know
 * when the button state changes.
 * Write to the LED Characteristic to change the state of the LED on the
 * board.
 *
 * @param[in] callbacks Struct containing pointers to callback functions
 *			used by the service. This pointer can be NULL
 *			if no callback functions are defined.
 *
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int bt_bs_init(struct bt_bs_cb *callbacks);

/** @brief Send the button state.
 *
 * This function sends a binary state, typically the state of a
 * button, to all connected peers.
 *
 * @param[in] button_state The state of the button.
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int bt_bs_send_button_state(bool button_state);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* BT_LBS_H_ */
