/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef BT_WS_H_
#define BT_WS_H_

/**@file
 * @defgroup bt_lbs LED Button Service API
 * @{
 * @brief API for the LED Button Service (LBS).
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/types.h>

/** @def BT_UUID_WS_VAL
 *  @brief Weight Scale Service UUID value
 */
#define BT_UUID_WS_VAL 0x181d
/** @def BT_UUID_WS
 *  @brief Weight Scale Service
 */
#define BT_UUID_WS \
	BT_UUID_DECLARE_16(BT_UUID_WS_VAL)

/** @def BT_UUID_WS_MEASUREMENT_VAL
 *  @brief WS Measurement Characteristic UUID value
 */
#define BT_UUID_WS_MEASUREMENT_VAL 0x2a9d
/** @def BT_UUID_TEMPERATURE
 *  @brief WS Measurement Characteristic
 */
#define BT_UUID_WS_MEASUREMENT \
	BT_UUID_DECLARE_16(BT_UUID_WS_MEASUREMENT_VAL)

/** @def BT_UUID_WS_FEATURE_VAL
 *  @brief WS Feature Characteristic UUID value
 */
#define BT_UUID_WS_FEATURE_VAL 0x2a9e
/** @def BT_UUID_TEMPERATURE
 *  @brief WS Feature Characteristic
 */
#define BT_UUID_WS_FEATURE \
	BT_UUID_DECLARE_16(BT_UUID_WS_FEATURE_VAL)




/** @brief LBS Service UUID. */
//#define BT_UUID_LBS_VAL \
//	BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)

/** @brief Button Characteristic UUID. */
//#define BT_UUID_LBS_BUTTON_VAL \
//	BT_UUID_128_ENCODE(0x00001524, 0x1212, 0xefde, 0x1523, 0x785feabcd123)

/** @brief LED Characteristic UUID. */
//#define BT_UUID_LBS_LED_VAL \
//	BT_UUID_128_ENCODE(0x00001525, 0x1212, 0xefde, 0x1523, 0x785feabcd123)


//#define BT_UUID_LBS           BT_UUID_DECLARE_128(BT_UUID_LBS_VAL)
//#define BT_UUID_LBS_BUTTON    BT_UUID_DECLARE_128(BT_UUID_LBS_BUTTON_VAL)
//#define BT_UUID_LBS_LED       BT_UUID_DECLARE_128(BT_UUID_LBS_LED_VAL)

/** @brief Callback type for when an LED state change is received. */
typedef void (*led_cb_t)(const bool led_state);

/** @brief Callback type for when the button state is pulled. */
typedef bool (*button_cb_t)(void);

/** @brief Callback struct used by the WS Service. */
struct bt_ws_cb {
	/** LED state change callback. */
	led_cb_t    led_cb;
	/** Button read callback. */
	button_cb_t button_cb;
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
int bt_ws_init(struct bt_ws_cb *callbacks);

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
int bt_ws_send_button_state(bool button_state);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* BT_LBS_H_ */
