#include "protocol.h"
#include <stdio.h>
#include <unistd.h>
#define sleep_time 0


void wait_for_event() {
    printf("Waiting for event");

    // Loop to print dots every second for a total of 3 seconds
    for (int i = 0; i < 3; ++i) {
        fflush(stdout);  // Flush the output buffer to ensure immediate printing
        usleep(sleep_time);        // Sleep for 1 second
        printf(".");
    }

    printf("\n\n");
}

void start_ack_timer(seq_nr k) {
    printf("[TRANSMITTER]: Start Acknowledge Time for sequence %i\n", k);
    usleep(sleep_time);
}

void stop_ack_timer(seq_nr k) {
    printf("[TRANSMITTER]: End Acknowledge Time for sequence %i\n", k);
    usleep(sleep_time);
}

void enable_network_layer(void) {
    printf("[TRANSMITTER]: Enable Network layer\n");
    usleep(sleep_time);
}

void disable_network_layer(void) {
    printf("[TRANSMITTER]: Disable Network layer\n");
    usleep(sleep_time);
}



void is_received(boolean ck, seq_nr frame_expected) {
    ck ? printf("[RECEIVER]: Data %i Received\n", frame_expected) : printf("[RECEIVER]: Data %i not received\n", frame_expected);
    usleep(sleep_time);
}

void from_network_layer(packet p) {
    printf("[TRANSMITTER]: Receiving Packet From Network Layer: Data=%s\n", p.data);
    usleep(sleep_time);
}

void to_network_layer(packet p) {
    printf("[RECEIVER]: Sending Packet To Network Layer: Data=%s\n", p.data);
    usleep(sleep_time);
}

void from_physical_layer(frame s) {
    printf("[RECEIVER]: Receiving Frame From Physical Layer: Seq=%d, Ack=%d, Data=%s\n", s.seq, s.ack, s.info.data);
    usleep(sleep_time);
}

void to_physical_layer(frame s) {
    printf("[TRANSMITTER]: Sending Frame To Physical Layer: Seq=%d, Ack=%d, Data=%s\n", s.seq, s.ack, s.info.data);
    usleep(sleep_time);
}

void start_timer(seq_nr k) {
    printf("[TRANSMITTER]: Start Timer for sequence number %i\n", k);
}

void stop_timer(seq_nr k) {
    printf("[TRANSMITTER]: End Timer for sequence number %i\n", k);
    usleep(sleep_time);
}

void displayEvent(event_type event) {
    switch (event) {
        case frame_arrival:
            printf("[EVENT]: Frame Arrival\n");
            break;
        case cksum_err:
        case time_out:
            printf("[EVENT]: %s Error\n", (event == cksum_err) ? "Checksum" : "Time out");
            break;
        default:
            printf("[EVENT]: Unknown\n");
    }
}