#ifndef PROTOCOL5_H
#define PROTOCOL5_H
#include "stdbool.h"
#include "protocol.h"
#define MAX_SEQ 100

void Serialize_to_packet(const char *sentence, packet words[MAX_PKT]);

void Serialize_to_frame(packet words[MAX_PKT], frame frames[MAX_PKT]);

void display_frames(const frame frames[MAX_PKT]);

void display_packets(const packet words[MAX_PKT]);

static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[]);

void protocol5(const char *sentence ,char result[MAX_PKT]);

boolean is_frame_expected(frame receiver[],seq_nr frame_expected);

void concatenate_frames(const frame frames[], int num_frames, char result[MAX_PKT]);


#endif // PROTOCOL5_H