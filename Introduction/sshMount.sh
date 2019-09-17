#!/usr/bin/env bash

mkdir -p ~/mnt/$1
sshfs $1: ~/mnt/$1
