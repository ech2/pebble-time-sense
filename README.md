Time Sense for Pebble
=================

Time Sense reminds of time passing by vibrating your Pebble watch at specified
time intervals. It works in the background, so you can use your watch for
anything else. Vibration interval lengths are configurable and can be in the
range from 1 hour down to 5 mins.

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
will continuosly attempt to schedule the vibration one minute later until it
finally succeed (number of attempts isn’t limited at the moment).

## Alternatives

There’re alternative apps that do vibration on specified intervals. Ironically,
I found them after I wrote this one.

1. [TimeStyle](https://github.com/freakified/TimeStylePebble) watchface has a
   setting that enables periodic vibration at either 1 hour or 30 minutes
   interval. Time Sense supports more interval options.
2. [Purr](https://github.com/jbrooksuk/Purr) does vibration every 5 mins. From
   what I can read in its source code, this app needs to be open in order to
   work. Time Sense works in background.
