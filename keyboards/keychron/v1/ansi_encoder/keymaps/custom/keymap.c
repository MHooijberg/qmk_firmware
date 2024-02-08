/* Copyright 2021 @ Keychron (https://www.keychron.com)
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

#include "quantum.h"
#include QMK_KEYBOARD_H

// clang-format off
enum custom_keycodes {
    ENC_CLCK = SAFE_RANGE,
    ENC_LFT,
    ENC_RGHT,
    LGUIT_NTR,
    LGUIT_SPC,
};

enum layers{
    DEFAULT,
    GAMING,
    PROGRAMMING,
    FN
};

enum modes{
    DESKTOP,
    PROGRAM,
    VOLUME,
    MUSIC
};

#define KC_FLXP LGUI(KC_E)
#define KC_TASK LGUI(KC_TAB)
#define NUM_OF_MODES 4

bool is_alt_tab_active = false; // ADD this near the beginning of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.
uint8_t current_mode = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
	case ENC_CLCK:
		if (record->tap.count && record->event.pressed) {
			switch (current_mode){
				case 0:
					tap_code16(G(KC_TAB)); // Show/hide windows task view.
					break;
				case 1:
					tap_code16(G(KC_D)); // Show/hide all windows.
					break;
				case 2:
					tap_code16(KC_MUTE); // Mute volume.
					break;
				case 3:
					tap_code16(KC_MPLY); // Pause/play music.
					break;
			}
		} else if (record->event.pressed) { // Define long press
			current_mode = (current_mode + 1) % NUM_OF_MODES; // Switch current encoder mode.

      uint8_t current_animation = rgb_matrix_get_mode(); // Get current animation.
      HSV current_color = rgb_matrix_get_hsv(); // Get current color.
      
      rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_SPLASH); // Set mode to reactive.
      rgb_matrix_sethsv_noeeprom(80 * current_mode, 255, 255); // Change color based on mode.
      
      rgb_matrix_toggle_noeeprom();

      rgb_matrix_mode(current_animation);
      rgb_matrix_sethsv(current_color.h, current_color.s, current_color.v);
		}
		return false;
	case ENC_LFT:
		switch (current_mode){
			case 0:
				tap_code16(C(G(KC_LEFT))); // Scroll one desktop left
				break;
			case 1:
				tap_code16(A(S(KC_TAB))); // Last program in queue
				break;
			case 2:
				tap_code16(KC_VOLD); // Volume down control.
				break;
			case 3:
				tap_code16(KC_MPRV); // Previous song.
				break;
		}
		return false;
	case ENC_RGHT:
		switch (current_mode){
			case 0:
				tap_code16(C(G(KC_RGHT))); // Next desktop.
				break;
			case 1: // Super Alt+Tab
        if (record->event.pressed) {
          if (!is_alt_tab_active) {
            is_alt_tab_active = true;
            register_code(KC_LALT);
          }
          alt_tab_timer = timer_read();
          register_code(KC_TAB);
        }
        else {
          unregister_code(KC_TAB);
        }
        return true; // Handle key normally?
			case 2:
				tap_code16(KC_VOLU); // Volume up control.
				break;
			case 3:
				tap_code16(KC_MNXT); // Next song.
				break;
		}
		return false;
		
    case LGUIT_NTR:
      if (record->event.pressed) {
        static bool currentKey = false;

        if (currentKey){
          tap_code16(G(KC_TAB));
        }
        else {
          tap_code16(KC_ENTER);
        }
        
        currentKey = !currentKey;
      }
      return false;
      
    case LGUIT_SPC:
      if (record->event.pressed) {
        static bool currentKey = false;

        if (currentKey){
          tap_code16(G(KC_TAB));
        }
        else {
          tap_code16(KC_SPACE);
        }
        
        currentKey = !currentKey;
      }
      return false;
  }
  return true;
}

void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEFAULT] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_PSCR,            ENC_CLCK,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_HOME,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_INS,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_DEL,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 MO(3),  DF(GAMING), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [PROGRAMMING] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_PSCR,            ENC_CLCK,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_HOME,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_INS,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_DEL,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 MO(3),  DF(GAMING), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [GAMING] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   A(KC_PSCR),         KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_F13,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_F14,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_F15,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 MO(3),  DF(PROGRAMMING), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [FN] = LAYOUT_ansi_82(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),
};

// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [DEFAULT] = { ENCODER_CCW_CW(ENC_LFT, ENC_RGHT)},
    [PROGRAMMING]   = { ENCODER_CCW_CW(ENC_LFT, ENC_RGHT)},
    [GAMING] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [FN]   = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI)}
};
#endif // ENCODER_MAP_ENABLE
