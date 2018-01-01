#!/bin/bash
# start openocd server
# you must run this script by root or with us permissions
#              @by pietrek.j@gmail.com
openocd -f /usr/share/openocd/scripts/interface/stlink-v2-1.cfg -f /usr/share/openocd/scripts/target/stm32f4x.cfg


