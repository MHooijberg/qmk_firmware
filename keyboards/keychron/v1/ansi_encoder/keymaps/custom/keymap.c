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
#include "print.h"

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
    Layer1,
    Layer2,
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

bool is_alt_tab_active = false;
bool is_enc_clck_active = false;
uint16_t alt_tab_timer = 0;
uint16_t enc_clck_timer = 0;
uint8_t current_mode = 0;
uint8_t t = 0;
uint8_t current_animation;
HSV current_color;

void handle_enc_click(void) {

  
  // rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_SPLASH); // Set mode to reactive.
  // rgb_matrix_sethsv_noeeprom(70 * current_mode, 255, 255); // Change color based on mode.
  // rgb_matrix_toggle_noeeprom();



  // rgb_matrix_mode_noeeprom(current_animation);
  // rgb_matrix_sethsv_noeeprom(current_color.h, current_color.s, current_color.v);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (is_enc_clck_active && timer_elapsed(enc_clck_timer) >= 300) {
    current_mode = (current_mode + 1) % NUM_OF_MODES; // Switch current encoder mode.
    is_enc_clck_active = false;
    return false;
  }

  switch (keycode) {
    case ENC_CLCK: 
      if (record->event.pressed && is_enc_clck_active == false)
      {
        is_enc_clck_active = true;
        enc_clck_timer = timer_read();
      }
      else {
        switch (current_mode){
          case 0: // Show/hide windows task view.
            tap_code16(G(KC_TAB)); 
            break;
          case 1: // Show/hide all windows.
            tap_code16(G(KC_D));
            break;
          case 2: // Mute volume.
            tap_code16(KC_MUTE); 
            break;
          case 3: // Pause/play music.
            tap_code16(KC_MPLY); 
            break;
        }
        is_enc_clck_active = false;
      }
      return false;
    case ENC_LFT:
      switch (current_mode){
        case 0: // Scroll one desktop left
          if (record->event.pressed) {
            tap_code16(C(G(KC_LEFT))); 
          }
          break;
        case 1: // Super Alt+Tab
          if (record->event.pressed) {
            if (!is_alt_tab_active) {
              is_alt_tab_active = true;
              register_code(KC_LALT);
            }
            alt_tab_timer = timer_read();
            register_code16(S(KC_TAB));
          } else {
            unregister_code16(S(KC_TAB));
          }
          return true;
        case 2: // Volume down control.
          if (record->event.pressed) {
          tap_code16(KC_VOLD); 
          }
          break;
        case 3: // Previous song.
          if (record->event.pressed) {
            tap_code16(KC_MPRV); 
          }
          break;
      }
      return false;
    case ENC_RGHT:
      switch (current_mode){
        case 0: // Next desktop.
          if (record->event.pressed) {
            tap_code16(C(G(KC_RGHT))); 
          }
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
          return true;
        case 2: // Volume up control.
          if (record->event.pressed) {
            tap_code16(KC_VOLU); 
          }
          break;
        case 3: // Next song.
          if (record->event.pressed) {
            tap_code16(KC_MNXT); 
          }
          break;
      }
      return false;
    }
    return true;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
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
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 MO(3),  DF(Layer1), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [Layer1] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_PSCR,            ENC_CLCK,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_HOME,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_INS,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_DEL,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 MO(3),  DF(Layer2), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [Layer2] = LAYOUT_ansi_82(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   A(KC_PSCR),         KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_F13,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_F14,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_F15,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 MO(3),  DF(DEFAULT), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [FN] = LAYOUT_ansi_82(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,            RGB_MODE_BREATHE,
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
    [FN]   = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)}
};
#endif // ENCODER_MAP_ENABLE
