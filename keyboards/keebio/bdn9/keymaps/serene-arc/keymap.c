/* Copyright 2019 Danny Nguyen <danny@keeb.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.	If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
bool enc_page_move = false;

enum custom_keycodes {
	TOGG_ENC = SAFE_RANGE,
};

// tapdance keycodes
enum td_keycodes {
	WIN_SWITCH,
	WIN_CLOSE,
	FFOX_TABCTRL,
	FIREFOX_TABS
};

// define a type containing as many tapdance states as you need
typedef enum {
	SINGLE_TAP,
	DOUBLE_TAP,
	TRIPLE_TAP
} td_state_t;

// create a global instance of the tapdance state type
static td_state_t td_state;

// function to determine the current tapdance state
int cur_dance (qk_tap_dance_state_t *state);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		TOGG_ENC, 		TD(FFOX_TABCTRL),	KC_MPLY,
		A(KC_W), 		A(C(KC_R)),		TD(FIREFOX_TABS),
		C(A(KC_1)), 	C(KC_PGDN),		C(KC_T)
	),

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case TOGG_ENC:
			if (record->event.pressed) {
				enc_page_move = !enc_page_move;
			}
	}
	return true;
}

void encoder_update_user(uint8_t index, bool clockwise) {
	if (index == 0) {
		if (enc_page_move) {
			if (clockwise) {
				tap_code16(C(KC_PGDN));
			} else {
				tap_code16(C(KC_PGUP));
			}
		} else{
			if (clockwise) {
				tap_code16(KC_PGDN);
			} else {
				tap_code16(KC_PGUP);
			}
		}
	}
	else if (index == 1) {
		if (clockwise) {
			tap_code(KC_VOLU);
		} else {
			tap_code(KC_VOLD);
		}
	}
}

void matrix_scan_user(void) {
	if (!is_alt_tab_active) {
		if (timer_elapsed(alt_tab_timer) > 700) {
			unregister_code(KC_LALT);
			is_alt_tab_active = false;
		}
	}
}

// determine the tapdance state to return
int cur_dance (qk_tap_dance_state_t *state) {
	if (state->count == 1) { return SINGLE_TAP; }
	if (state->count == 2) { return DOUBLE_TAP; }
	if (state->count == 3) { return TRIPLE_TAP; }
	else { return 3; } // any number higher than the maximum state value you return above
}

void winswitch_fun (qk_tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			if (!is_alt_tab_active) {
				is_alt_tab_active = true;
				register_code(KC_LALT);
			}
			alt_tab_timer = timer_read();
			tap_code(KC_F6);
			break;
		case DOUBLE_TAP:
			tap_code16(A(KC_TAB));
			break;
		default:
			break;
	}
}

void winclose_fun (qk_tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			tap_code16(KC_F11);
			break;
		case DOUBLE_TAP:
			tap_code16(A(KC_F4));
			break;
		default:
			break;
	}
}

void ffox_tabctrl_func (qk_tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			tap_code16(C(KC_W));
			break;
		case DOUBLE_TAP:
			tap_code16(S(C(KC_T)));
			break;
		default:
			break;
	}
}

void firetab_func (qk_tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			tap_code16(C(KC_L));
			break;
		case DOUBLE_TAP:
			tap_code16(KC_BSPACE);
			break;
		case TRIPLE_TAP:
			tap_code16(S(KC_BSPACE));
			break;
		default:
			break;
	}
}

// define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
qk_tap_dance_action_t tap_dance_actions[] = {
	[WIN_SWITCH] = ACTION_TAP_DANCE_FN(winswitch_fun),
	[WIN_CLOSE] = ACTION_TAP_DANCE_FN(winclose_fun),
	[FIREFOX_TABS] = ACTION_TAP_DANCE_FN(firetab_func),
	[FFOX_TABCTRL] = ACTION_TAP_DANCE_FN(ffox_tabctrl_func)
};
