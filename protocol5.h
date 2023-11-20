#ifndef PROTOCOL5_H
#define PROTOCOL5_H
#include "stdbool.h"
#include "protocol.h"
#define MAX_SEQ 7

void Serialize_to_packet(const char *sentence, packet words[MAX_PKT]);

void Serialize_to_frame(packet words[MAX_PKT], frame frames[MAX_PKT]);

void display_frames(const frame frames[MAX_PKT]);

void display_packets(const packet words[MAX_PKT]);

static bool between(seq_nr a, seq_nr b, seq_nr c);

static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[], frame Receiver[],event_type *event);

void protocol5(const char *sentence ,char result[MAX_PKT]);

event_type getRandomEvent();

boolean is_frame_expected(frame receiver[],seq_nr frame_expected);

void concatenate_frames(const frame frames[], int num_frames, char result[MAX_PKT]);


#endif // PROTOCOL5_H