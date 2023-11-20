//
// Created by zyn66 on 11/19/2023.
//
#include "stdbool.h"
#include "../protocol/protocal.h"
#define MAX_SEQ 7

void Serialize_to_packet(const char *sentence, packet words[MAX_PKT]);

void Serialize_to_frame(packet words[MAX_PKT], frame frames[MAX_PKT]);

void display_frames(const frame frames[MAX_PKT]);

void display_packets(const packet words[MAX_PKT]);

static bool between(seq_nr a, seq_nr b, seq_nr c);

static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[], frame Receiver[],event_type event);

void protocol5(const char *sentence);

event_type getRandomEvent();

boolean is_frame_expected(frame receiver[],seq_nr frame_expected);