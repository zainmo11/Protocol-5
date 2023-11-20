#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#include "protocol5.h"
#include <string.h>
/* Protocol 5 (Go-back-n) allows multiple outstanding frames. The sender may transmit up
to MAX SEQ frames without waiting for an ack. In addition, unlike in the previous
protocols, the network layer is not assumed to have a new packet all the time. Instead,
the network layer causes a network layer ready event when there is a packet to send. */

void Serialize_to_packet(const char *sentence, packet words[MAX_PKT]) {
    // Create a temporary copy of the sentence to avoid modifying the original string
    char sentenceCopy[1000];
    strcpy(sentenceCopy, sentence);

    // Use strtok to tokenize the sentence based on space characters
    char *token = strtok(sentenceCopy, " ");
    int i = 0;

    // Loop through the tokens and copy them into the words array
    while (token != NULL && i < MAX_PKT) {
        strcpy(words[i].data, token);
        token = strtok(NULL, " ");
        i++;
    }
}

void Serialize_to_frame(packet words[MAX_PKT], frame frames[MAX_PKT]) {
    int i = 0;

    // Loop through the words and copy them into the frames array
    while (i < MAX_PKT && words[i].data[0] != '\0') {
        frames[i].kind = data;   // Assuming all are data frames
        frames[i].seq = i;             // Sequence number
        frames[i].ack = 0;             // Acknowledgement number (assuming it's not used for data frames)
        strcpy(frames[i].info.data, words[i].data);  // Copy the packet data into the frame
        i++;
    }

    // Ensure the last entry in the frames array is empty
    frames[i].kind = data;
    frames[i].seq = i;
    frames[i].ack = 0;
    frames[i].info.data[0] = '\0';
}

void display_frames(const frame frames[MAX_PKT]) {
    for (int i = 0; i < MAX_PKT && frames[i].info.data[0] != '\0'; i++) {
        printf("Frame %d:\n", i + 1);
        printf("  Kind: %d\n", frames[i].kind);
        printf("  Seq: %d\n", frames[i].seq);
        printf("  Ack: %d\n", frames[i].ack);
        printf("  Data: %s\n", frames[i].info.data);
        printf("\n");
    }
}

void display_packets(const packet words[MAX_PKT]) {
    printf("Packets:\n");
    for (int i = 0; i < MAX_PKT && words[i].data[0] != '\0'; ++i) {
        printf("Packet %d: %s\n", i + 1, words[i].data);
    }
}


static bool between(seq_nr a, seq_nr b, seq_nr c)
{
/* Return true if a <= b < c circularly; false otherwise. */
    if (((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)))
        return(true);
    else
        return(false);
}


static void Receive_data(frame s, frame frames[], seq_nr frame_nr, event_type *e)
{
    /*to randomize the sequence of sending Frames */
    event_type randx = (event_type)(rand() % 2);
    *e = randx;
    if (randx == frame_arrival) {
        frames[frame_nr] = s;
    }
}

static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[], frame Receiver[], event_type *event)
{
    /* Construct and send a data frame. */
    frame s; /* scratch variable */
    s.info = buffer[frame_nr]; /* insert packet into frame */
    s.seq = frame_nr; /* insert sequence number into frame */
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);/* piggyback ack */
    to_physical_layer(s); /* transmit the frame */
    Receive_data(s, Receiver, frame_nr, event);
    start_timer(frame_nr); /* start the timer running */
}

boolean is_frame_expected(frame receiver[],seq_nr frame_expected) {
    for (int i = 0; i <= MAX_SEQ; i++) {
        if (receiver[i].seq == frame_expected) {
            return true;
        }
    }
    return false;
}
event_type getRandomEvent() {
    return (event_type)(rand() % 5);
}

void concatenate_frames(const frame frames[], int num_frames, char result[MAX_PKT]) {
    result[0] = '\0';  // Initialize the result string

    for (int i = 0; i < num_frames; ++i) {
        if (frames[i].kind == data) {
            strcat(result, frames[i].info.data);  // Concatenate the data field
        }
    }
}


void protocol5(const char *sentence, char result[MAX_PKT]) {
    packet SenderPackets[MAX_PKT];
    frame SenderFrames[MAX_PKT], receivedFrames[MAX_PKT];

    Serialize_to_packet(sentence, SenderPackets);
    Serialize_to_frame(SenderPackets, SenderFrames);
    display_packets(SenderPackets);
    display_frames(SenderFrames);

    seq_nr next_frame_to_send = 0;
    seq_nr ack_expected = 0;
    seq_nr frame_expected = 0;
    seq_nr nbuffered = 0;
    seq_nr i;
    event_type event;

    enable_network_layer();

    from_network_layer(SenderPackets[next_frame_to_send]);
    nbuffered = nbuffered + 1;
    send_data(next_frame_to_send, frame_expected, SenderPackets, receivedFrames, &event);
    inc(next_frame_to_send);

    while (true) {
        wait_for_event(&event);

        switch (event) {
            case frame_arrival:
                from_physical_layer(receivedFrames[next_frame_to_send - 1]);
                if (is_frame_expected(receivedFrames, frame_expected)) {
                    to_network_layer(receivedFrames[frame_expected].info);
                    inc(frame_expected);
                    nbuffered--;
                    stop_timer(ack_expected);
                    inc(ack_expected);
                }
                break;

            case cksum_err_or_time_out:
                next_frame_to_send = ack_expected;
                for (i = 1; i <= nbuffered; i++) {
                    send_data(next_frame_to_send, frame_expected, SenderPackets, receivedFrames, &event);
                    inc(next_frame_to_send);
                }
                break;
        }

        if (nbuffered < MAX_SEQ)
            enable_network_layer();
        else
            disable_network_layer();

        if (SenderFrames[next_frame_to_send].info.data[0] == 0) {
            break;
        }

        from_network_layer(SenderPackets[next_frame_to_send]);
        nbuffered = nbuffered + 1;
        send_data(next_frame_to_send, frame_expected, SenderPackets, receivedFrames, &event);
        inc(next_frame_to_send);
    }

    concatenate_frames(receivedFrames, MAX_PKT, result);
}


//    while (i){
//        if (receivedFrames[ack_expected].seq == frame_expected){
//            to_network_layer(receivedFrames[ack_expected].info);
//            inc(frame_expected);
//            while (between(ack_expected, r .ack, next_frame_to_send)) {
//                /* Handle piggybacked ack. */
//                nbuffered--; /* one frame fewer buffered */
//                stop_timer(ack_expected); /* frame arrived intact; stop timer */
//                inc(ack_expected); /* contract sender’s window */
//        }
//
//    }