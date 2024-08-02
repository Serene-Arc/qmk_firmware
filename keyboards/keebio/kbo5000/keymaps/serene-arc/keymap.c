#include QMK_KEYBOARD_H

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
uint16_t alt_tab_limit = 600;

// define a type containing as many tapdance states as you need
typedef enum {
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_TAP,
  DOUBLE_HOLD,
  TRIPLE_TAP,
} td_state_t;

//Tap Dance Declarations
enum {
	TD_SPC_SENT = 1,
	DISC_DN,
	DISC_UP,
	TD_BRACK_O,
	TD_BRACK_C,
	LATEX_E,
	LATEX_S,
};

enum custom_keycodes {
	CLEAR_MOD = SAFE_RANGE,
	WIN_SWCH,
	A_ESC,
	VIM_N1,
	VIM_N2,
	VIM_N3,
	VIM_N4,
	VIM_N5,
	VIM_N6,
	VIM_N7,
	TAB_SWTH,
	ENQUOTE,
	EMPH,
	LATEX_CHAP,
    MATH_I,
    LATEX_T,
    LATEX_B,
    LATEX_U,
    MATH_1,
    MATH_2,
    ALIGN_EQ,
    ESC_UND,
    MINT_IN,
};

// create a global instance of the tapdance state type
static td_state_t td_state;

// function to determine the current tapdance state
int cur_dance (tap_dance_state_t *state);

enum encoder_names {
  LEFT_HALF_ENC,
  RIGHT_HALF_ENC1,
  RIGHT_HALF_ENC2,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_all(
	CLEAR_MOD,	KC_F1,	 KC_F2, KC_F3,	 KC_F4,   KC_F5,   KC_F6,					 KC_F7,   KC_F8,			KC_F9,	 KC_F10,  KC_F11,  KC_F12,	KC_PSCR, KC_MNXT, KC_MPLY,
	WIN_SWCH,KC_ESC,  KC_1,    KC_2,	KC_3,	 KC_4,	  KC_5,    KC_6,			 KC_7,	  KC_8,    KC_9,	KC_0,	 KC_MINS, KC_EQL,  KC_BSPC, KC_DEL, KC_INS,  KC_PGUP,
	A_ESC,	 KC_TAB,  KC_Q,    KC_W,	KC_E,	 KC_R,	  KC_T,						 KC_Y,	  KC_U,    KC_I,	KC_O,	 KC_P,	  KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,  KC_PGDN,
	C(A(KC_T)),   KC_NO, KC_A,	  KC_S, KC_D,	 KC_F,	  KC_G,					 KC_H,	  KC_J,    KC_K,	KC_L,	 KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,  KC_HOME, KC_END,
	TAB_SWTH,  SC_LSPO, KC_GRV,  KC_Z,	KC_X,	 KC_C,	  KC_V,    KC_B,			 KC_N,	  KC_M,    KC_COMM, KC_DOT,  KC_SLSH,		   SC_RSPC,			 KC_UP,
	KC_MS_BTN3,  KC_LCTL, TD(TD_BRACK_O), KC_LGUI, MO(1),	 SFT_T(KC_ENT),  KC_SPC,	 MO(1),   TD(TD_SPC_SENT),	TD(TD_BRACK_C), KC_LGUI, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [1] = LAYOUT_all(
	QK_BOOT,			  MATH_1, MATH_2, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD,			 _______, _______,			_______, _______, _______, _______, _______, _______, _______,
	RGB_TOG, _______, VIM_N1, VIM_N2, VIM_N3, VIM_N4, VIM_N5, VIM_N6,			 _______, _______, _______, _______, ESC_UND, ALIGN_EQ, _______, _______, _______, _______,
	RGB_MOD, _______, _______, _______, TD(LATEX_E), _______, LATEX_T,					 _______, LATEX_U, MATH_I, _______, _______, _______, _______, _______, _______, _______,
	_______, KC_CAPS, _______, TD(LATEX_S), _______, _______, _______,					 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
	_______, _______, _______, _______, _______, LATEX_CHAP, _______, LATEX_B,			 _______, MINT_IN, _______, _______, _______,		   _______,			 TD(DISC_UP),
	_______, _______, _______, _______, _______, _______, _______,					 _______, _______, _______, _______,				   _______, KC_MPRV, TD(DISC_DN), KC_MNXT
  ),

};

bool encoder_update_user(uint8_t index, bool clockwise) {
	if (index == LEFT_HALF_ENC) {
		if (clockwise) {
		tap_code16(KC_PGDN);
		} else {
		tap_code16(KC_PGUP);
		}
		} else if (index == RIGHT_HALF_ENC1) {
		if (clockwise) {
			tap_code16(KC_VOLU);
		} else {
			tap_code16(KC_VOLD);
		}
	} else if (index == RIGHT_HALF_ENC2) {
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
	}
	return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case A_ESC:
			if (record->event.pressed) {
				if (!is_alt_tab_active) {
					is_alt_tab_active = true;
					register_code(KC_LALT);
				}
				alt_tab_timer = timer_read();
				tap_code16(KC_ESC);
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
		case LATEX_CHAP:
			if (record->event.pressed) {
				SEND_STRING("\\chapter{");
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
		case LATEX_T:
			if (record->event.pressed) {
				SEND_STRING("\\texttt{");
			}
			break;
		case LATEX_U:
			if (record->event.pressed) {
				SEND_STRING("\\underline{");
			}
			break;
		case LATEX_B:
			if (record->event.pressed) {
				SEND_STRING("\\bm{");
			}
			break;
		case MATH_1:
			if (record->event.pressed) {
				SEND_STRING("^{-1}");
			}
			break;
		case MATH_2:
			if (record->event.pressed) {
				SEND_STRING("^2");
			}
			break;
		case ESC_UND:
			if (record->event.pressed) {
				SEND_STRING("\\_");
			}
			break;
		case MINT_IN:
			if (record->event.pressed) {
				SEND_STRING("\\mintinline{");
			}
			break;
		case ALIGN_EQ:
			if (record->event.pressed) {
				SEND_STRING("&=");
			}
			break;
		case MATH_I:
			if (record->event.pressed) {
				SEND_STRING("_i");
			}
			break;
		case CLEAR_MOD:
			if (record->event.pressed) {
				clear_oneshot_mods();
				clear_mods();
				clear_keyboard();
				led_t state = host_keyboard_led_state();
				if (state.caps_lock == true)
					tap_code16(KC_CAPS);
			}
			break;
		case TAB_SWTH:
			if (record->event.pressed) {
				if (get_mods() & MOD_MASK_SHIFT){
					int current = get_mods();
					del_mods(MOD_MASK_SHIFT);
					tap_code16(C(KC_PGUP));
					set_mods(current);
				} else {
					tap_code16(C(KC_PGDN));
				}
			}
			break;
		default:
			break;
	}
	return true;
}

void matrix_scan_user(void) {
	if (is_alt_tab_active) {
		if (timer_elapsed(alt_tab_timer) > alt_tab_limit) {
			unregister_code(KC_LALT);
			is_alt_tab_active = false;
		}
	}
}

int cur_dance (tap_dance_state_t *state) {
	if (state->count == 1) {
		if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
		else { return SINGLE_HOLD; }
	}
	if (state->count == 2) {
		if (state->interrupted || !state->pressed) { return DOUBLE_TAP; }
		else { return DOUBLE_HOLD; }
	}
	if (state->count == 3) { return TRIPLE_TAP; }
	else { return 99; } // any number higher than the maximum state value you return above
}

void td_spacesent_finished (tap_dance_state_t *state, void *user_data){
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

void td_spacesent_reset (tap_dance_state_t *state, void *user_data){
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

void td_ctrlbracketclosed_finished (tap_dance_state_t *state, void *user_data){
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			tap_code16(S(KC_RBRC));
			break;
		case SINGLE_HOLD:
			register_code16(KC_LALT);
			break;
		default:
			break;
	}
}

void td_ctrlbracketclosed_reset (tap_dance_state_t *state, void *user_data){
	switch (td_state) {
		case SINGLE_HOLD:
			unregister_code16(KC_LALT);
			break;
		default:
			break;
	}
}

void td_ctrlbracketopen_finished(tap_dance_state_t *state, void *user_data){
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			tap_code16(S(KC_LBRC));
			break;
		case SINGLE_HOLD:
			register_code16(KC_LALT);
			break;
		default:
			break;
	}
}

void td_ctrlbracketopen_reset (tap_dance_state_t *state, void *user_data){
	switch (td_state) {
		case SINGLE_HOLD:
			unregister_code16(KC_LALT);
			break;
		default:
			break;
	}
}

void discdown_fun (tap_dance_state_t *state, void *user_data) {
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

void discup_fun (tap_dance_state_t *state, void *user_data) {
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

void latex_e_func (tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			SEND_STRING("\\enquote{");
			break;
		case DOUBLE_TAP:
			SEND_STRING("\\emph{");
			break;
		default:
			break;
	}
}

void latex_s_func (tap_dance_state_t *state, void *user_data) {
	td_state = cur_dance(state);
	switch (td_state) {
		case SINGLE_TAP:
			SEND_STRING("\\section{");
			break;
		case DOUBLE_TAP:
			SEND_STRING("\\subsection{");
			break;
		case TRIPLE_TAP:
			SEND_STRING("\\subsubsection{");
			break;
		default:
			break;
	}
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case TD(TD_SPC_SENT):
			return 500;
		case TD(TD_BRACK_O):
		case TD(TD_BRACK_C):
			return 120;
		default:
			return 200;
	}
}

tap_dance_action_t tap_dance_actions[] = {
	[DISC_DN] = ACTION_TAP_DANCE_FN(discdown_fun),
	[DISC_UP] = ACTION_TAP_DANCE_FN(discup_fun),
	[TD_SPC_SENT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_spacesent_finished, td_spacesent_reset),
	[TD_BRACK_O] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_ctrlbracketopen_finished, td_ctrlbracketopen_reset),
	[TD_BRACK_C] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_ctrlbracketclosed_finished, td_ctrlbracketclosed_reset),
	[LATEX_E] = ACTION_TAP_DANCE_FN(latex_e_func),
	[LATEX_S] = ACTION_TAP_DANCE_FN(latex_s_func),
};
