#include "protocol.h"
#include <stdio.h>
#include <unistd.h>
#define sleep_time 50


void wait_for_event() {
    printf("Waiting for event");

    // Loop to print dots every second for a total of 3 seconds
    for (int i = 0; i < 3; ++i) {
        fflush(stdout);  // Flush the output buffer to ensure immediate printing
        usleep(sleep_time);        // Sleep for 1 second
        printf(".");
    }
    printf("\n");
}

void start_ack_timer(void) {
    printf("Start Acknowledge Time for sequence number\n");
    usleep(sleep_time);
}

void stop_ack_timer(void) {
    printf("End Acknowledge Time for sequence number\n");
    usleep(sleep_time);
}

void enable_network_layer(void) {
    printf("Enable Network layer\n");
    usleep(sleep_time);
}

void disable_network_layer(void) {
    printf("Disable Network layer\n");
    usleep(sleep_time);
}


/*
 todo :
   Wrapping a function with another function that takes a variable number of frames and starts from the beginning of the given frame.
 */

void is_received(event_type e, frame frame_expected) {
    e == frame_arrival ? printf("[RECEIVER]: Data %s Received\n", frame_expected.info.data) : printf("[RECEIVER]: Data %i not received\n", frame_expected.seq);
    usleep(sleep_time);
}

void from_network_layer(packet p) {
    printf("[TRANSMITTER]: Get Packet '%s' from network layer \n", p.data);
    usleep(sleep_time);
}

void to_network_layer(packet p) {
    printf("[RECEIVER]: Send Packet '%s' to network layer\n", p.data);
    usleep(sleep_time);
}

void from_physical_layer(frame r) {
    printf("[RECEIVER]: Get frame '%s' from physical layer\n", r.info.data);
    usleep(sleep_time);
}

void to_physical_layer(frame s) {
    printf("[TRANSMITTER]: Send frame '%s' to physical layer\n", s.info.data);
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
