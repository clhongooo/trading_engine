
# CME MDP Intro
[CME MDP](http://www.cmegroup.com/confluence/display/EPICSANDBOX/CME+Market+Data)
[CME vs FAST](http://ftw.edu.wwx.tw/Home/kgq/gmds-wen-zhang-suo-yin/gmds/cmemdp30deguihuayujianzhi)


# Packet Structure
[CME MDP Message Structure](http://www.cmegroup.com/confluence/display/EPICSANDBOX/MDP+3.0+-+Packet+and+Message+Headers)
[Real-Logic implementation of SBE](https://github.com/real-logic/simple-binary-encoding)
[Real-Logic C++ guide](https://github.com/real-logic/simple-binary-encoding/wiki/Cpp-User-Guide)


## HKEx OMD

| Field    | Length  | Description                                        |
| :---     | :---    |                                                    |
| PktSize  | uint16  | Size of the packet (including this field)          |
| MsgCount | uint8   | Number of messages included in the packet          |
| Filler   | char(1) |                                                    |
| SeqNum   | uint32  | Sequence number of the first message in the packet |
| SendTime | uint64  | UTC Unix Timestamp in nanoseconds.                 |

## CME Messages

[Market Data Snapshot Full Refresh](http://www.cmegroup.com/confluence/display/EPICSANDBOX/MDP+3.0+-+Market+Data+Snapshot+Full+Refresh)

# Major System Components
-   Multicast Receiver
-   Data Completeness Inspector
-   RTS Client
-   PreProcessor
-   Real Time Channel Processor
-   Refresh Channel Processor

# System Implementations
-  OMDC
-  OMDD
-  MDP  (Normal mode and Fast mode for algo trading)

# Program Flow

For each channel, there are 2 multicast streams for real-time and 2 for refresh altogether 4 streams,
Each stream is handled by 1 Multicast Receiver, and 1 PreProcessor.
Each channel is handled by 1 RealTimeProcessor and 1 RefreshProcessor

E.g. Channel 10

    RealTime A  -  McastReceiver  -  PreProcessor  -
                                                    |- RealTime Processor
    RealTime B  -  McastReceiver  -  PreProcessor  -
    
    Refresh  A  -  McastReceiver  -  PreProcessor  -
                                                    |- Refresh Processor
    Refresh  B  -  McastReceiver  -  PreProcessor  -


## McastReceiver
-   Simply stores received data into circular buffers as is.
-   1 channel consists of 4 multicast streams `RT_A RT_B RF_A RF_B`,
-   so there are 4 McastReceivers for each of them.

## PreProcessor
-   Optionally records binary data or parses data into JSON format.
-   Handles sequence reset
-   Breaks packets down to messages and inserts them into another circular buffer according to their sequence number.

## Line Arbitration
-   Performed automatically when messages are inserted into the circle buffer by PreProcessor.
-   Dirty flag exists in circle buffer to indicate if a particular sequence no is already present, in which case the message won't be memcpy'd for the second time.

## Data Completeness Inspector
-   Continuously monitor circular buffers to determine if recovery options are needed.
-   When determined necessary, triggers retransmission client or switch a channel into refresh mode.
-   Recovery can be triggered by sequence number gap or inactive time gap.

## RealTime / Refresh Processor
-   There is 1 Data Processor for each multicast channel.

## Refresh
-   To minimize the time needed to finish refresh, the data in refresh channels are continuously listened to and buffered up
-   And the buffered data is purged if the "Refresh Complete" message is received and refresh mode isn't triggered.
-   The buffered data will be processed when "Refresh Complete" message is received and refresh mode is triggered.
-   There are several cases where the system has to wait for the next refresh batch
    1.  There isn't enough data in the real time channel to continue from the specified LastSeqNum in the "Refresh Complete" message.
    1.  There is missing sequence number in that particular refresh batch.
-   Real time channel processing is paused until the whole refresh process is completed.

### Refresh Triggering
-   When sequence number gap is larger than threshold.
-   When time gap is larger than threshold.
-   When RTS reports errors like "Message not available". Since RTS doesn't have the message, there is no point in waiting further. So immediately activate refresh mode.

## Circular buffer
-   Memory is reused as much as possible to avoid the time spent on memory allocation.
-   Multicast data is directly written into circular buffers; To save copying time, memcpy is only used when absolutely necessary.

## Sequence number reset:
-   Each stream has its own local copy of the *sequence number offset* that is added to the sequence number of every message arrived.
    E.g. Each of the RT channel A and B has its own local copy of the *sequence number offset*.
-   There is a shared object for the *sequence number offset* in each channel.
-   When a stream receives the sequence reset message, it will update both its local copy and the shared object.
-   When a stream detects that the sequence number is from 1 - 10, it will check the shared object, just to make sure it hasn't missed a sequence number reset.
-   This method can at least handle these cases:
    1.   RT A is the faster stream. RT A has received the sequence reset message but RT B has missed it.
    2.   RT A is the faster stream. RT A has missed the sequence reset message but RT B has received it.
    3.   RT A is the faster stream. Both RT A and RT B have received the sequence reset message.
-   This method won't handle this case:
    1.   both RT A and RT B have missed the sequence reset message.

# Other Features
-   Can turn on / off the recording of raw binary data at each component.
-   Can turn on / off the outputing of parsed data as JSON at each component.
-   Log is written using the Pantheios library.
-   Can issue manual commands (e.g. To force refresh to be performed) into the system by telnet'ing the port 8000.
-   Conditional variable and is used for signaling among threads to achieve maximum speed


# Comparing with Hanh's CME MDP
- single thread vs numerous dedicated thread. will the latter be really slow? seems no.
- Hanh: allocate again and again the refresh buffer
- Hanh: refresh channel drop until got missing packets, not fast enough
- Hanh: no simultaneous handling of multiple channels / symbols, no good
- Sunny: (much) faster order book processing
- Sunny: lavish approach: prefer speed at the expense of system resource
