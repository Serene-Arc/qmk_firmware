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
	TD_SPC_SENT
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
	VIM_UP, 
	VIM_DOWN,
	VIM_LEFT,
	VIM_RIGHT,
	VIM_CLOSE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[_QWERTY] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐							 ┌────────┬────────┬────────┬────────┬────────┬────────┐
	 KC_ESC,  KC_1,    KC_2,	KC_3,	 KC_4,	  KC_5,								  KC_6,    KC_7,	TD(TD_8_AST),	 KC_9,	  KC_0,    KC_BSPC,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 KC_TAB,  KC_Q,    KC_W,	KC_E,	 KC_R,	  KC_T,								  KC_Y,    KC_U,	KC_I,	 KC_O,	  KC_P,    KC_BSLS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 OSM(MOD_LCTL), KC_A,    KC_S,	KC_D,	 KC_F,	  KC_G,							  KC_H,    KC_J,	KC_K,	 KC_L,	  KC_SCLN, KC_QUOT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐		┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
	 OSM(MOD_LSFT), KC_Z,    KC_X,	KC_C,	 KC_V,	  KC_B,	CLEAR_MOD,		 KC_MPLY,  KC_N,    KC_M,	KC_COMM, KC_DOT,  KC_SLSH, KC_ENTER,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘		└───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
									KC_LGUI, MO(_LOWER), SFT_T(KC_ENT),	 	 TD(TD_SPC_SENT),  MO(_RAISE),   OSM(MOD_LALT)
								// └────────┴────────┴────────┘					└────────┴────────┴────────┘
	),

	[_LOWER] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐							 ┌────────┬────────┬────────┬────────┬────────┬────────┐
	 KC_TILD, KC_EXLM, KC_AT,	KC_HASH, KC_DLR,  KC_PERC,							  KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 _______,   KC_1,    KC_2,	KC_3,	 KC_4,	  KC_5,								  KC_6,    VIM_CLOSE,VIM_UP,	 A(KC_F6),KC_PGDN, KC_PGUP,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 KC_DEL,  _______, KC_LEFT, KC_RGHT, KC_UP,   KC_LBRC,							  KC_RBRC, KC_P4,	VIM_LEFT, VIM_RIGHT, KC_PLUS, KC_HOME,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐		┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
	 BL_STEP, KC_GRV,   _______, _______, KC_DOWN, KC_LCBR, KC_LPRN,		 KC_RPRN, KC_RCBR, KC_P1,	VIM_DOWN, KC_P3,   KC_MINS, KC_END,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘		└───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
									_______, _______, KC_DEL,					 KC_DEL,  _______, KC_P0
								// └────────┴────────┴────────┘					└────────┴────────┴────────┘
	),

	[_RAISE] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐							 ┌────────┬────────┬────────┬────────┬────────┬────────┐
	 KC_F12,  KC_F1,   KC_F2,	KC_F3,	 KC_F4,   KC_F5,							  KC_F6,   KC_F7,	KC_F8,	 KC_F9,   KC_F10,  KC_F11,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 RESET,   KC_EXLM, KC_AT,	KC_HASH, KC_DLR,  KC_PERC,							  KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 RGB_MOD, KC_MPRV, SGUI(KC_LEFT), SGUI(KC_RIGHT), KC_PGUP, KC_UNDS,				  KC_EQL,  KC_HOME, RGB_HUI, RGB_SAI, RGB_VAI, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐		┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
	 S(KC_INS), KC_MUTE, KC_MPLY, KC_VOLD, KC_PGDN, KC_MINS, KC_LPRN,		 _______, KC_PLUS, KC_END,	RGB_HUD, RGB_SAD, RGB_VAD, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘		└───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
									_______, _______, _______,					 _______, _______, _______
								// └────────┴────────┴────────┘					└────────┴────────┴────────┘
	),

	[_ADJUST] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐							 ┌────────┬────────┬────────┬────────┬────────┬────────┐
	 _______, _______, _______, _______, _______, _______,							  _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 _______, _______, _______, _______, _______, _______,							  _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤							 ├────────┼────────┼────────┼────────┼────────┼────────┤
	 _______, _______, _______, _______, _______, _______,							  _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐		┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
	 _______, _______, _______, _______, _______, _______, _______,			 _______, _______, _______, _______, _______, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘		└───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
									_______, _______, _______,					 _______, _______, _______
								// └────────┴────────┴────────┘					└────────┴────────┴────────┘
	)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case CLEAR_MOD:
			clear_oneshot_mods();
			clear_mods();
			break;
		case VIM_UP:
			tap_code16(C(KC_W));
			tap_code16(KC_K);
			break;
		case VIM_DOWN:
			tap_code16(C(KC_W));
			tap_code16(KC_J);
			break;
		case VIM_LEFT:
			tap_code16(C(KC_W));
			tap_code16(KC_H);
			break;
		case VIM_RIGHT:
			tap_code16(C(KC_W));
			tap_code16(KC_L);
			break;
		case VIM_CLOSE:
			tap_code16(C(KC_W));
			tap_code16(KC_Q);
			break;
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
		if (timer_elapsed(alt_tab_timer) > 1000) {
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

void td_spacsent_fun (qk_tap_dance_state_t *state, void *user_data){
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			tap_code16(KC_SPACE);
			break;
		case DOUBLE_TAP:
			SEND_STRING(". ");
			break;
		default:
			break;
	}
}

qk_tap_dance_action_t tap_dance_actions[] = {
	[TD_8_AST]  = ACTION_TAP_DANCE_DOUBLE(KC_8, KC_PAST),
	[TD_SPC_SENT] = ACTION_TAP_DANCE_FN(td_spacsent_fun) 
};
