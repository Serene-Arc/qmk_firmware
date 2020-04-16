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

enum custom_keycodes {
	ALT_TAB = SAFE_RANGE,
};

// tapdance keycodes
enum td_keycodes {
	DISCORD_DOWN,
	DISCORD_UP,
	WIN_SWITCH,
	WIN_CLOSE
};

// define a type containing as many tapdance states as you need
typedef enum {
	SINGLE_TAP,
	DOUBLE_TAP
} td_state_t;

// create a global instance of the tapdance state type
static td_state_t td_state;

// function to determine the current tapdance state
int cur_dance (qk_tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void discdown_fun (qk_tap_dance_state_t *state, void *user_data);
void discup_fun (qk_tap_dance_state_t *state, void *user_data);
void winswitch_fun (qk_tap_dance_state_t *state, void *user_data);
void winclose_fun (qk_tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	//Window movement
	[0] = LAYOUT(
		LSFT(KC_PAUSE),	TD(WIN_CLOSE),		KC_MPLY,
		KC_NO,			SGUI(KC_RIGHT),		TG(2),
		TD(WIN_SWITCH),	SGUI(KC_LEFT),		TG(1)
	),
	
	//Program movement - Discord
	[1] = LAYOUT(
		KC_TRNS,	C(KC_K),			KC_TRNS,
		TO(0),		TD(DISCORD_UP),		LCA(KC_UP),
		KC_TRNS,	TD(DISCORD_DOWN),	LCA(KC_DOWN)
	),
	// Program movement - Firefox
	[2] = LAYOUT(
		KC_TRNS, C(KC_W),		KC_TRNS,
		TO(0),	 C(KC_PGUP),	C(KC_L),
		KC_TRNS, C(KC_PGDN),	C(KC_T)
	),
};

void encoder_update_user(uint8_t index, bool clockwise) {
	if (index == 0) {
		if (clockwise) {
			if (!is_alt_tab_active) {
				is_alt_tab_active = true;
				register_code(KC_LALT);
			}
			alt_tab_timer = timer_read();
			tap_code(KC_TAB);
		} else {
			if (!is_alt_tab_active) {
				is_alt_tab_active = true;
				register_code(KC_LALT);
			}
			alt_tab_timer = timer_read();
			tap_code16(LSFT(KC_TAB));
		}
	}
	else if (index == 1) {
		if (clockwise) {
			tap_code(KC_PGDN);
		} else {
			tap_code(KC_PGUP);
		}
	}
}

void matrix_scan_user(void) {
	if (timer_elapsed(alt_tab_timer) > 750) {
		unregister_code(KC_LALT);
		is_alt_tab_active = false;
	}
}



// determine the tapdance state to return
int cur_dance (qk_tap_dance_state_t *state) {
	if (state->count == 1) { return SINGLE_TAP; }
	if (state->count == 2) { return DOUBLE_TAP; }
	else { return 3; } // any number higher than the maximum state value you return above
}

void discdown_fun (qk_tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
            tap_code16(A(KC_DOWN));
            break;
		case DOUBLE_TAP:
            tap_code16(S(A(KC_DOWN)));
    }
}

void discup_fun (qk_tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
            tap_code16(A(KC_UP));
            break;
		case DOUBLE_TAP:
            tap_code16(S(A(KC_UP)));
    }
}

void winswitch_fun (qk_tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
            tap_code16(A(KC_F6));
            break;
		case DOUBLE_TAP:
            tap_code16(A(KC_TAB));
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
    }
}

// define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
qk_tap_dance_action_t tap_dance_actions[] = {
	[DISCORD_DOWN] = ACTION_TAP_DANCE_FN(discdown_fun),
	[DISCORD_UP] = ACTION_TAP_DANCE_FN(discup_fun),
	[WIN_SWITCH] = ACTION_TAP_DANCE_FN(winswitch_fun),
	[WIN_CLOSE] = ACTION_TAP_DANCE_FN(winclose_fun)
};
