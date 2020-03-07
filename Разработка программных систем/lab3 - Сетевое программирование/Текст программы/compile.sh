#!/bin/bash

cc client/client.c client/elevator.c -o client/lift -pthread
cc server/server.c -o server/server -pthread