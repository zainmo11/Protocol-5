# Protocol 5 (Go-Back-N) 



## Tasks

- The sender send four frames to receiver 
- while true 
- if the receiver received the frame 
- receiver sed ack to sender 
- elseif receiver didn't receive the frame 
-  receiver send NAK of frame lost to sender 
-elseif receiver checksum wroung
- receiver send all four frames
- if sender frames finished 
- sender send frames  from nak  request frame or if it received correct send next four frames
- else break

## Overview

This implementation demonstrates Protocol 5, also known as Go-Back-N, for reliable communication over a network. The protocol allows for multiple outstanding frames, where the sender can transmit up to `MAX_SEQ` frames without waiting for an acknowledgment. Unlike previous protocols, Protocol 5 doesn't assume a constant stream of packets from the network layer but relies on a network layer ready event.

## Files and Structure

- **protocol5.h:** Header file containing declarations for Protocol 5 functions and structures.
- **protocol5.c:** Implementation file containing the Protocol 5 logic.
- **protocol.h:** Header file containing declarations for base Protocol functions and structures.
- **protocol.c:** Implementation file containing the base Protocol logic.
  
## Protocol Description

### Overview
Protocol 5 uses a sliding window approach to manage the transmission of frames. It handles various events such as the arrival of frames, checksum errors, timeouts, and network layer readiness.

### Functions
- **protocol5():** The main function implementing Protocol 5. It handles the transmission and reception of frames, manages the sliding window, and reacts to different events.
- **send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[]):** Constructs and sends a data frame. It inserts the packet into the frame, includes the sequence number, and piggybacks the acknowledgment.

### Static Functions
- **between(seq_nr a, seq_nr b, seq_nr c):** Checks if `b` is between `a` and `c` circularly.

## Usage

### 1-Compilation
```bash
gcc main.c -o program.exe
```
### 2-Execution
```bash
program.exe
```
## Customization
-You can customize the protocol behavior and constants by modifying the relevant sections in the code, such as adjusting MAX_SEQ or updating the packet structure definition
- you can initialize input that will be a message that will sent
- test every case for Protocol 5
## License
-This software is licensed under the MIT License.
