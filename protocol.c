#include "protocol.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

// Assuming timer is a global variable
boolean timer = false;

void wait_for_event(event_type *event) {
    printf("Waiting for event\n");

    // Loop to print dots every second for a total of 3 seconds
    for (int i = 0; i < 3; ++i) {
        fflush(stdout);  // Flush the output buffer to ensure immediate printing
        sleep(1);        // Sleep for 1 second
        printf(".");
    }
    printf("\n");
}

void is_received(event_type e, seq_nr frame_expected) {
    e == frame_arrival ? printf("Data %i Received\n", frame_expected) : printf("Data %i not received\n", frame_expected);
    sleep(1);
}

void from_network_layer(packet p) {
    printf("Take Packet %s from network layer to physical layer\n", p.data);
    sleep(1);
}

void to_network_layer(packet p) {
    printf("Take Packet %s from physical layer to network layer\n", p.data);
    sleep(1);
}

void from_physical_layer(frame r) {
    printf("Take frame %s to network layer\n", r.info.data);
    sleep(1);
}

void to_physical_layer(frame s) {
    printf("Take frame %s to network layer\n", s.info.data);
    sleep(1);
}

void start_timer(seq_nr k) {
    printf("Start Timer for sequence number %i\n", k);
    timer = true;
}

void stop_timer(seq_nr k) {
    printf("End Timer for sequence number %i\n", k);
    timer = false;
    sleep(1);
}

void start_ack_timer(void) {
    printf("Start Acknowledge Time for sequence number\n");
    timer = true;
    sleep(1);
}

void stop_ack_timer(void) {
    printf("End Acknowledge Time for sequence number\n");
    timer = false;
    sleep(1);
}

void enable_network_layer(void) {
    printf("Enable Network layer\n");
    sleep(1);
}

void disable_network_layer(void) {
    printf("Disable Network layer\n");
    sleep(1);
}

void displayEvent(event_type event) {
    switch (event) {
        case frame_arrival:
            printf("Event: Frame Arrival\n");
            break;
        case cksum_err_or_time_out:
            printf("Event: Checksum or Time out Error\n");
            break;
        default:
            printf("Unknown Event\n");
    }
}
