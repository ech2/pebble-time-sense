Time Sense for Pebble
=================

An app that reminds of time passing by vibrating your Pebble watch at specified
time intervals.

## Installation

> Note: Pebble watches are currently supported by the community, located at
> [rebble.io](http://rebble.io). Developer information was moved to
> [developer.rebble.io](https://developer.rebble.io/developer.pebble.com/index.html).

> These instructions have only been tested on Arch Linux.

1. Follow the instructions:
   https://developer.rebble.io/developer.pebble.com/sdk/install/linux/index.html
2. Activate the Pebble’s *virtualenv*:
   ```
   cd ~/pebble-dev/pebble-sdk-4.5-linux64
   source .env/bin/activate
   ```
3. Create file `NO_TRACKING` in `~/.pebble-sdk` and install SDK core files
   from an archive repository (official Pebble’s servers doesn’t work anymore):
   ```
   mkdir -p ~/.pebble-sdk
   touch ~/.pebble-sdk/NO_TRACKING
   pebble sdk install https://github.com/aveao/PebbleArchive/raw/master/SDKCores/sdk-core-4.3.tar.bz2
   ```
4. Enable *Developer Connection* in the Pebble Android / iOS app.
5. Clone this repo and install this app:
   ```
   git clone https://github.com/ech2/pebble-time-sense
   cd pebble-time-sense
   pebble build && pebble install --phone {phone_ip}
   ```
6. Now you can deactivate *virtualenv*:
   ```
   deactivate
   ```

## Usage

This app doesn’t have any interface. Just select it and press middle button to
change time intervals. Supported intervals are: “Every hour”, “Every 30 mins”,
“Every 15 mins”, “Every 10 mins”, “Every 5 mins”. For example, “Every 15 mins”
mean that Pebble will vibrate on 15-th, 30-th and 45-th minute of each hour.

Currently, if another event has been scheduled on the same minute, Time Sense
will continue to try to schedule the vibration one minute later until it
succeeds (number of tries isn’t limited at the moment).
