/* Protocol 5 (Go-back-n) allows multiple outstanding frames. The sender may transmit up
to MAX SEQ frames without waiting for an ack. In addition, unlike in the previous
protocols, the network layer is not assumed to have a new packet all the time. Instead,
the network layer causes a network layer ready event when there is a packet to send. */

#include <stdio.h>
#include "protocol.h"
#include "protocol5.h"
#include <string.h>
#include <stdlib.h>

#define MAX_FRAMES 7

int end_frames = 0;
event_type events[MAX_FRAMES + 1];
frame s; /* Scratch variable */


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
    words[i].data[0] = '\0';
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
    end_frames = i;
}

void display_frames(const frame frames[MAX_PKT]) {
    printf("Frames:\n");

    int maxFrameNumberWidth = 2;  // Adjust according to the number of frames

    for (int i = 0; i < MAX_PKT && frames[i].info.data[0] != '\0'; i++) {
        // Use dynamic width for frame numbers and fixed width for other columns
        printf("Frame %*d:  Kind: %d  Seq: %d  Ack: %d  Data: %s\n",
               maxFrameNumberWidth, i + 1, frames[i].kind, frames[i].seq, frames[i].ack, frames[i].info.data);
    }
}

void display_packets(const packet words[MAX_PKT]) {
    printf("Packets:\n");

    int maxPacketNumberWidth = 2;  // Adjust according to the number of packets
    int dataWidth = 20;

    for (int i = 0; i < MAX_PKT && words[i].data[0] != '\0'; ++i) {
        // Use dynamic width for packet numbers and fixed width for data
        printf("Packet %*d: %-*s", maxPacketNumberWidth, i + 1, dataWidth, words[i].data);

        // Print newline after every fourth column
        if ((i + 1) % 2 == 0 || words[i + 1].data[0] == '\0') {
            printf("\n");
        }
    }
}

void concatenate_frames(const frame frames[], int num_frames, char result[MAX_PKT]) {
    result[0] = '\0';  // Initialize the result string

    for (int i = 0; i < num_frames; ++i) {
        if (frames[i].kind == data) {
            strcat(result, frames[i].info.data);  // Concatenate the data field
            strcat(result," ");
        }
    }
}


static void Receive_data(frame scratch, frame frames[], int frame_nr, event_type *e) {
    // Seed the random number generator with the current time
    static int initialized = 0;
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }

    // Generate a random number between 0 and 99
    int randNum = rand() % 100;

    // Set the event based on the random number
    // 60% for frame_arrival
    if (randNum < 60) {
        *e = frame_arrival;
        frames[frame_nr] = scratch;
    } else if (randNum < 85) {
        *e = cksum_err;
    } else {
        *e = time_out;
    }
}

static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[])
{
    /* Construct and send a data frame. */
    s.info = buffer[frame_nr]; /* insert packet into frame */
    s.seq = frame_nr; /* insert sequence number into frame */
    s.ack = (frame_expected==0)? 0 : (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);/* piggyback ack */
    s.kind = data;
    to_physical_layer(s); /* transmit the frame */
}

boolean is_frame_expected(frame receiver[],seq_nr frame_expected) {
    for (int i = 0; i < MAX_SEQ; i++) {
        if (receiver[i].seq == frame_expected) {
            return true;
        }
    }
    return false;
}

static void send_multiple_frames(seq_nr *frame_nr, seq_nr frame_expected, packet buffer[], frame Receiver[], event_type *event){

    for(int i = 0; i < MAX_FRAMES; i++){
        from_network_layer(buffer[*frame_nr]);
        send_data(*frame_nr, frame_expected, buffer);
        Receive_data(s, Receiver, *frame_nr, event);
        events[i] = *event;
        start_timer(*frame_nr); /* start the timer running */
        start_ack_timer(*frame_nr);
        displayEvent(*event);
        inc(*frame_nr);
        if((*frame_nr) == end_frames) {
            *frame_nr = *frame_nr + (MAX_FRAMES - (i + 1));
            break;
        }
    }

    *event = events[0];
    printf("\n");
}

static void is_expected(frame receiver[],seq_nr frame_expected, seq_nr *frame_nr, seq_nr *n_buf,
                        seq_nr *ack_expect, event_type *e){

    for(int i = 0; i < MAX_FRAMES; i++){
        if((*frame_nr - MAX_FRAMES + i ) == end_frames) {
            *n_buf = *n_buf - (MAX_FRAMES - i);
            break;
        }
        if (is_frame_expected(receiver, frame_expected) && events[i] == frame_arrival) {
            from_physical_layer(receiver[*frame_nr - MAX_FRAMES + i]);
            displayEvent(events[i]);
            is_received(True, *frame_nr - MAX_FRAMES + i);
            to_network_layer(receiver[*frame_nr - MAX_FRAMES + i].info);
            stop_ack_timer(*frame_nr - MAX_FRAMES + i);
            stop_timer(*frame_nr - MAX_FRAMES + i);
            inc(frame_expected);
            *n_buf = *n_buf - 1;
            inc(*ack_expect);
        }
        else {
            *n_buf = *n_buf - (MAX_FRAMES - i);
            *e = events[i];
            displayEvent(events[i]);
            is_received(False, *frame_nr - MAX_FRAMES + i);
            break;
        }

    }
    printf("\n");

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
    event_type event;

    enable_network_layer();
    nbuffered = nbuffered + 4;
    send_multiple_frames(&next_frame_to_send,
                         frame_expected,
                         SenderPackets,
                         receivedFrames,
                         &event);


    while (true) {
        wait_for_event();


        switch (event) {
            case frame_arrival:
                is_expected(receivedFrames,
                            frame_expected,
                            &next_frame_to_send,
                            &nbuffered, &ack_expected, &event);

                next_frame_to_send = ack_expected;
                break;


            case cksum_err:
            case time_out:
                stop_ack_timer(ack_expected);
                next_frame_to_send = ack_expected;
                send_multiple_frames(&next_frame_to_send,
                                     frame_expected,
                                     SenderPackets,
                                     receivedFrames,
                                     &event);
                wait_for_event();
                is_expected(receivedFrames,
                            frame_expected,
                            &next_frame_to_send,
                            &nbuffered, &ack_expected, &event);
                next_frame_to_send = ack_expected;
                break;
        }

        if (SenderFrames[next_frame_to_send].info.data[0] == 0) {
            break;
        }



        if (nbuffered < MAX_SEQ)
            enable_network_layer();
        else
            disable_network_layer();


        nbuffered = nbuffered + 4;
        send_multiple_frames(&next_frame_to_send,
                             frame_expected,
                             SenderPackets,
                             receivedFrames,
                             &event);



    }
    display_frames(receivedFrames);
    concatenate_frames(receivedFrames, MAX_PKT, result);
}
