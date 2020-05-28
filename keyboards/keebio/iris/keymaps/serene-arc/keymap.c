#include QMK_KEYBOARD_H


#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

//Tap Dance Declarations
enum {
	TD_8_AST = 0,
	TD_SPC_SENT, 
	DISC_DN, 
	DISC_UP
};

// define a type containing as many tapdance states as you need
typedef enum {
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_TAP
} td_state_t;

// create a global instance of the tapdance state type
static td_state_t td_state;

// function to determine the current tapdance state
int cur_dance (qk_tap_dance_state_t *state);

enum custom_keycodes {
	CLEAR_MOD = SAFE_RANGE,
	WIN_SWCH,
	VIM_UP, 
	VIM_DOWN,
	VIM_LEFT,
	VIM_RIGHT,
	VIM_CLOSE,
	VIM_N1,
	VIM_N2,
	VIM_N3,
	VIM_N4,
	VIM_N5,
	VIM_N6,
	VIM_N7,
	VIM_CMLPT
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[_QWERTY] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐							 ┌────────┬────────┬────────┬────────┬────────┬────────┐
	 KC_ESC,  KC_1,    KC_2,	KC_3,	 KC_4,	  KC_5,								  KC_6,    KC_7,	KC_8,	 KC_9,	  KC_0,    KC_BSPC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 KC_TAB,  KC_Q,    KC_W,	KC_E,	 KC_R,	  KC_T,								  KC_Y,    KC_U,	KC_I,	 KC_O,	  KC_P,    KC_BSLS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 OSM(MOD_LCTL), KC_A,    KC_S,	KC_D,	 KC_F,	  KC_G,							  KC_H,    KC_J,	KC_K,	 KC_L,	  KC_SCLN, KC_QUOT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐		┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
	 OSM(MOD_LSFT), KC_Z,    KC_X,	KC_C,	 KC_V,	  KC_B,	CLEAR_MOD,		 KC_MPLY,  KC_N,    KC_M,	KC_COMM, KC_DOT,  KC_SLSH, KC_SFTENT,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘		└───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
									KC_LGUI, MO(_LOWER), SFT_T(KC_ENT),	 	 TD(TD_SPC_SENT),  MO(_RAISE),   OSM(MOD_LALT)
								// └────────┴────────┴────────┘					└────────┴────────┴────────┘
	),

	[_LOWER] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐							 ┌────────┬────────┬────────┬────────┬────────┬────────┐
	 KC_TILD, KC_EXLM, KC_AT,	KC_HASH, KC_DLR,  KC_PERC,							  KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 _______,  KC_DEL,    KC_2,	KC_3,	 KC_4,	  KC_5,								  KC_6,    VIM_CLOSE,VIM_UP, WIN_SWCH, KC_PGDN, KC_PGUP,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 _______, KC_DOT,	KC_LEFT, KC_RGHT, KC_UP,   KC_LBRC,							  KC_RBRC, KC_P4,	VIM_LEFT, VIM_RIGHT, KC_PLUS, KC_HOME,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐		┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
	 _______, KC_GRV,   _______, _______, KC_DOWN, KC_LCBR, KC_LPRN,		 KC_RPRN, KC_RCBR, KC_LGUI,	VIM_DOWN, KC_P3,   KC_MINS, KC_END,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘		└───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
									_______, _______, KC_DEL,					 KC_DEL,  _______, KC_EQL
								// └────────┴────────┴────────┘					└────────┴────────┴────────┘
	),

	[_RAISE] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐							 ┌────────┬────────┬────────┬────────┬────────┬────────┐
	 KC_F12,  KC_F1,   KC_F2,	KC_F3,	 KC_F4,   KC_F5,							  KC_F6,   KC_F7,	KC_F8,	 KC_F9,   KC_F10,  KC_F11,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 VIM_CMLPT, VIM_N1, VIM_N2,	VIM_N3,	 VIM_N4,  VIM_N5,							  VIM_N6, VIM_N7,	KC_ASTR, KC_LPRN, KC_RPRN, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 KC_PGDN, KC_MNXT, LCA(KC_DOWN), LCA(KC_UP), TD(DISC_UP), KC_MINS,				  KC_EQL,  KC_HOME, RGB_HUI, RGB_SAI, RGB_VAI, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐		┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
	 S(KC_INS), KC_MUTE, KC_MPLY, KC_VOLD, TD(DISC_DN), KC_UNDS, KC_LPRN,	 _______, KC_PLUS, KC_END,	RGB_HUD, RGB_SAD, RGB_VAD, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘		└───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
									_______, _______, KC_DOT,					 _______, _______, _______
								// └────────┴────────┴────────┘					└────────┴────────┴────────┘
	),

	[_ADJUST] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐							 ┌────────┬────────┬────────┬────────┬────────┬────────┐
	 _______, _______, _______, _______, _______, _______,							  _______, _______, _______, _______, _______, KC_PSCR,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 RESET,    _______, _______, _______, _______, _______,							  _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 _______, _______, SGUI(KC_LEFT), SGUI(KC_RIGHT), _______, _______,				  _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐		┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
	 BL_STEP, _______, _______, _______, _______, _______, _______,			 _______, _______, _______, _______, _______, _______, _______,
	 //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘		└───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
	 _______, _______, _______,					 _______, _______, _______
	 // └────────┴────────┴────────┘					└────────┴────────┴────────┘
	 )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case CLEAR_MOD:
			if (record->event.pressed) {
				clear_oneshot_mods();
				clear_mods();
			}
			break;
		case VIM_UP:
			if (record->event.pressed) {
				tap_code16(C(KC_W));
				tap_code16(KC_K);
			}
			break;
		case VIM_DOWN:
			if (record->event.pressed) {
				tap_code16(C(KC_W));
				tap_code16(KC_J);
			}
			break;
		case VIM_LEFT:
			if (record->event.pressed) {
				tap_code16(C(KC_W));
				tap_code16(KC_H);
			}
			break;
		case VIM_RIGHT:
			if (record->event.pressed) {
				tap_code16(C(KC_W));
				tap_code16(KC_L);
			}
			break;
		case VIM_CLOSE:
			if (record->event.pressed) {
				tap_code16(C(KC_W));
				tap_code16(KC_Q);
			}
			break;
		case WIN_SWCH:
			if (record->event.pressed) {
				if (!is_alt_tab_active) {
					is_alt_tab_active = true;
					register_code(KC_LALT);
				}
				alt_tab_timer = timer_read();
				tap_code16(KC_F6);
			}
			break;
		case VIM_N1:
			if (record->event.pressed) {
				SEND_STRING("$!nm1");
			}
			break;
		case VIM_N2:
			if (record->event.pressed) {
				SEND_STRING("$!nm2");
			}
			break;
		case VIM_N3:
			if (record->event.pressed) {
				SEND_STRING("$!nm3");
			}
			break;
		case VIM_N4:
			if (record->event.pressed) {
				SEND_STRING("$!nm4");
			}
			break;
		case VIM_N5:
			if (record->event.pressed) {
				SEND_STRING("$!nm5");
			}
			break;
		case VIM_N6:
			if (record->event.pressed) {
				SEND_STRING("$!nm6");
			}
			break;
		case VIM_N7:
			if (record->event.pressed) {
				SEND_STRING("$!nm7");
			}
			break;
		case VIM_CMLPT:
			if (record->event.pressed) {
				register_code(KC_LCTRL);
				tap_code(KC_X);
				tap_code(KC_F);
				unregister_code(KC_LCTRL);
			}
		default:
			break;
	}
	return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

void encoder_update_user(uint8_t index, bool clockwise) {
	if (index == 0) {
		if (layer_state_is(_QWERTY)) {
			if (!is_alt_tab_active) {
				is_alt_tab_active = true;
				register_code(KC_LALT);
			}
			if (clockwise) {
				alt_tab_timer = timer_read();
				tap_code16(KC_TAB);
			} else {
				alt_tab_timer = timer_read();
				tap_code16(S(KC_TAB));
			}
		} else if (layer_state_is(_RAISE)) {
			if (clockwise) {
				tap_code(KC_DOWN);
			} else {
				tap_code(KC_UP);
			}
		} else if (layer_state_is(_LOWER)) {
			if (!is_alt_tab_active) {
				is_alt_tab_active = true;
				register_code(KC_LALT);
			}
			if (clockwise) {
				alt_tab_timer = timer_read();
				tap_code16(KC_GRV);
			}
			else {
				alt_tab_timer = timer_read();
				tap_code16(S(KC_GRV));
			}
		}
	}
	else if (index == 1) {
		if (layer_state_is(_QWERTY)) {
			if (clockwise) {
				tap_code(KC_PGDN);
			} else {
				tap_code(KC_PGUP);
			}
		} else if (layer_state_is(_LOWER)) {
			if (clockwise) {
				tap_code(KC_RIGHT);
			} else {
				tap_code(KC_LEFT);
			}
		}
	}
}
void matrix_scan_user(void) {
	if (is_alt_tab_active) {
		if (timer_elapsed(alt_tab_timer) > 750) {
			unregister_code(KC_LALT);
			is_alt_tab_active = false;
		}
	}
}

int cur_dance (qk_tap_dance_state_t *state) {
	if (state->count == 1) {
		if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
		else { return SINGLE_HOLD; }
	}
	if (state->count == 2) { return DOUBLE_TAP; }
	else { return 3; } // any number higher than the maximum state value you return above
}

void td_spacesent_finished (qk_tap_dance_state_t *state, void *user_data){
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			register_code(KC_SPACE);
			break;
		case SINGLE_HOLD:
			register_code(KC_SPACE);
			break;
		case DOUBLE_TAP:
			SEND_STRING(". ");
			break;
		default:
			break;
	}
}

void td_spacesent_reset (qk_tap_dance_state_t *state, void *user_data){
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			unregister_code(KC_SPACE);
			break;
		case SINGLE_HOLD:
			unregister_code(KC_SPACE);
			break;
		case DOUBLE_TAP:
			break;
		default:
			break;
	}
}

void discdown_fun (qk_tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			tap_code16(A(KC_DOWN));
			break;
		case DOUBLE_TAP:
			tap_code16(S(A(KC_DOWN)));
			break;
		default:
			break;
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
			break;
		default:
			break;
	}
}

qk_tap_dance_action_t tap_dance_actions[] = {
	[DISC_DN] = ACTION_TAP_DANCE_FN(discdown_fun),
	[DISC_UP] = ACTION_TAP_DANCE_FN(discup_fun),
	[TD_8_AST]  = ACTION_TAP_DANCE_DOUBLE(KC_8, KC_PAST),
	[TD_SPC_SENT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_spacesent_finished, td_spacesent_reset) 
};
