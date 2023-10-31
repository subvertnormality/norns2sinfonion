#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

#define SYNC_BUFFER_SIZE 6
#define HARMONIC_SYNC_TX_IRQ_TIMEOUT 1000

#define LED_PIN 13

uint8_t buffer[SYNC_BUFFER_SIZE] = {0};
unsigned long last_interrupt = 0;
uint8_t buffer_index  = 0;
bool wait_for_sync = true;
uint8_t last_clock = 0;
uint8_t last_beat = 0;
uint8_t last_step = 0;
uint8_t last_reset = 0;

void debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}


void set_root_note(uint8_t root) {
    buffer[0] = (buffer[0] & ~0x0f) | (root % 12);
}

uint8_t rootNote() {
    return buffer[0] & 0x0f;
}

void set_degree_nr(uint8_t degree_nr) {
    buffer[1] = (buffer[1] & ~0x0f) | degree_nr;
}

uint8_t degree_nr() {
    return buffer[1] & 0x0f;
}

void set_mode_nr(uint8_t mode_nr) {
    buffer[2] = (buffer[2] & ~0x0f) | mode_nr;
}

uint8_t mode_nr() {
    return buffer[2] & 0x0f;
}

void set_clock(uint8_t clock) {
    buffer[0] = (buffer[0] & ~0x70) | ((clock % 8) << 4);
}

uint8_t get_clock() {
    return (buffer[0] & 0x70) >> 4;
}

void set_transposition(int8_t trans) {
    trans = max(-64, min(63, trans));
    buffer[3] = (buffer[3] & ~0x7f) | ((trans + 64) & 0x7f);
}

int8_t transposition() {
    return (buffer[3] & 0x7f) - 64;
}

void set_chaotic_detune(float detune) {
    detune = max(-1.0, min(1.0, detune));
    int8_t detune_int = floor(detune * 63.0) + 63;
    buffer[4] = detune_int & 0x7f;
}

float chaotic_detune() {
    int8_t detune_int = buffer[4] - 63;
    return detune_int / 63.0;
}

void set_harmonic_shift(int8_t shift) {
    buffer[5] = shift + 16;
}

int8_t harmonic_shift() {
    return buffer[5] - 16;
}

void set_beat(uint8_t beat) {
    buffer[1] = (buffer[1] & ~0x70) | ((beat % 8) << 4);
}

uint8_t beat() {
    return (buffer[1] & 0x70) >> 4;
}

void set_step(uint8_t step) {
    buffer[2] = (buffer[2] & ~0x70) | ((step % 8) << 4);
}

uint8_t step() {
    return (buffer[2] & 0x70) >> 4;
}

void set_reset(uint8_t reset_value) {
    buffer[5] = (buffer[5] & ~0x60) | ((reset_value % 4) << 5);
}

uint8_t reset() {
    return (buffer[5] & 0x60) >> 5;
}

void handle_rx_irq(uint8_t byte) {
    last_interrupt = millis();
    if (byte & 0x80) {
        wait_for_sync = false;
        buffer_index = 0;
        buffer[0] = byte;
    } else if (!wait_for_sync) {
        buffer_index++;
        buffer[buffer_index] = byte;
    }
}

uint8_t handle_tx_irq() {
    last_interrupt = millis();
    uint8_t byte = buffer[buffer_index ];
    buffer_index  = (buffer_index  + 1) % SYNC_BUFFER_SIZE;
    return byte;
}

void dump() {
    for (int i = 0; i < SYNC_BUFFER_SIZE; i++) {
        debug("%02x ", buffer[i]);
    }
    debug("\n");
}

void send_next() {
    Serial2.write(buffer[buffer_index ]);
    buffer_index  = (buffer_index + 1) % SYNC_BUFFER_SIZE;
}

void handleSinfonionCall(byte channel, byte program) {
    switch(channel) {
        case 1: 
            set_root_note(program);
            break;
        case 2: 
            set_degree_nr(program);
            break;
        case 3: 
            set_mode_nr(program);
            break;
        case 4: 
            set_transposition(program);
            break;
        case 5: 
            set_clock(program);
            break;
        case 6: 
            set_beat(program);
            break;
        case 7: 
            set_step(program);
            break;
        case 8: 
            set_reset(program);
            break;
        case 9: 
            set_chaotic_detune(program);
            break;
        case 10: 
            set_harmonic_shift(program);
            break;
        default:
            // Unknown channel, do nothing or handle error
            break;
    }
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    usbMIDI.setHandleProgramChange(handleSinfonionCall);
    Serial2.begin(115200, SERIAL_8N1_RXINV_TXINV);
    // Initialize buffer
    for (int i = 0; i < SYNC_BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }
    buffer[0] = 0x80;
}

void loop() {
    usbMIDI.read();
    send_next();
}
