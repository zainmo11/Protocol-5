//
// Created by zyn66 on 11/19/2023.
//
#include "stdbool.h"
#include "../protocol/protocal.h"
#define MAX_SEQ 7



static bool between(seq_nr a, seq_nr b, seq_nr c);

static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[]);

void protocol5(void);
