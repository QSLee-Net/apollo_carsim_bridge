# APOLLO_CARSIM_Bridge

## Introduction

This module provides a way for other Apollo modules interactiving with process
outside of Apollo by socket. It includes sender and receiver components.

## Input

In sender component, there is only one input, which is the proto struct sender
handled. In receiver comonent, its input is different with others. Its input
comes from UDP socket.

## Output

Sender's output is by way of UDP socket. Receiver has one output which is
receiver handled.

## Launch

Bridge contains two sub-modules, other than: sender and receiver. The sender is
responsible for sending the udp data, and the receiver is responsible for
receiving the data packet sent by the udp client, and parsing it, and then
sending it to the module that has registered to respond to the message. 

The start command of Sender is as follows.

```
$ ./apollo_carsim_bridge_send
```

The start command of Receiver is as follows:

```
$ ./apollo_carsim_bridge_receive
```


