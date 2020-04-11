/* Copyright 2019 Danny Nguyen <danny@keeb.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

enum custom_keycodes {
	ALT_TAB = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	//Window movement
    [0] = LAYOUT(
        LSFT(KC_PAUSE),	G(KC_PGDN),			KC_MPLY,
        LCA(KC_T), 		SGUI(KC_LEFT),		TG(1),
        A(KC_F6), 		SGUI(KC_RIGHT), 	TG(2)
    ),
    
	//Program movement - Discord
    [1] = LAYOUT(
        KC_TRNS,    C(KC_K),	KC_TRNS,
        TO(0), 		A(KC_UP),	LCA(KC_UP),
        KC_TRNS, 	A(KC_DOWN), LCA(KC_DOWN)
    ),
   	// Program movement - Firefox
    [2] = LAYOUT(
        KC_TRNS, C(KC_W),		KC_TRNS,
        TO(0),	 C(KC_PGDN),	C(KC_L),
        KC_TRNS, C(KC_PGUP),	C(KC_T)
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
