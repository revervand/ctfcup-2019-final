#!/bin/sh

socat -d TCP4-LISTEN:1338,reuseaddr,fork,keepalive exec:./main.py
