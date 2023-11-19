//
// Created by zyn66 on 11/18/2023.
//
#include "protocal.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
void wait_for_event(int *event){
    printf("Waiting for event");

    // Loop to print dots every second for a total of 3 seconds
    for (int i = 0; i < 3; ++i) {
        fflush(stdout);  // Flush the output buffer to ensure immediate printing
        sleep(1);        // Sleep for 1 second
        printf(".");
    }
    printf("\n");
};

void is_received(bool status){
    status ? printf("Data Received") : printf("Data not received");
    sleep(1);
}

void from_network_layer(packet *p){
    printf("Take Packet %s from network layer to physical layer\n",p->data);
    sleep(1);
};


void to_network_layer(packet *p){
    printf("Take Packet %s from physical layer to network layer]\n",p->data);
    sleep(1);
};


void from_physical_layer(frame *r){
    printf("Take frame %r to network layer",r->info);
    sleep(1);
};


void to_physical_layer(frame *s){
    printf("Take frame %r to network layer",s->info);
    sleep(1);
};


void start_timer(seq_nr k){
    printf("Start Timer for sequence number %i ",k);
    timer = True;
};


void stop_timer(seq_nr k){
    printf("End Timer for sequence number %i ",k);
    timer = False;
    sleep(1);
};


void start_ack_timer(void){
    printf("Start Acknowledge Time for sequence number");
    timer = True;
    sleep(1);
};


void stop_ack_timer(void){
    printf("End Acknowledge Time for sequence number");
    timer = True;
    sleep(1);
};


void enable_network_layer(void){
    printf("Enable Network layer");
    sleep(1);
};




void disable_network_layer(void){
    printf("Disable Network layer");
    sleep(1);
};


