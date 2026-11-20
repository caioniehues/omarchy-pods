# omarchy-pods

An Omarchy shell plugin and daemon that brings AirPods features (battery, listening modes, ear detection, codec selection) to Linux.

## Audio

**Playback profile**:
The card profile carrying high-quality stereo out to the AirPods, without a microphone; the daemon picks one by the user's codec choice.
_Avoid_: A2DP mode, music mode

**Headset profile**:
A card profile that carries a microphone (mSBC or CVSD) and replaces the playback profile; never selected on the user's behalf.
_Avoid_: HFP mode, call mode

**Hi-res mic**:
The AirPods microphone carried over Apple's AACP channel alongside the playback profile, without a headset profile.
_Avoid_: AirPods input, LE mic

## Bluetooth

**Bonded LE device**:
Any other Bluetooth Low Energy peripheral paired to the machine (mouse, keyboard) that must reconnect on its own while the daemon is looking for the AirPods.
_Avoid_: HID device, other device

**Airtime**:
The share of the AirPods' Bluetooth link a stream occupies; the playback profile and the hi-res mic draw on the same link.
_Avoid_: bandwidth, bitrate

## Fork

**Build branch**:
The fork's `build` branch: upstream plus every carried PR and our own fixes, the version actually installed.
_Avoid_: main, our branch

**Carried PR**:
An upstream PR applied to the build branch with its author's commits intact and our fixes as separate commits on top.
_Avoid_: patched PR, our version
