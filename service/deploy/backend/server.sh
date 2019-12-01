#!/bin/sh

socat -d TCP4-LISTEN:1337,reuseaddr,fork,keepalive exec:./ecs_backend