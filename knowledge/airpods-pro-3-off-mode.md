---
type: reference
title: AirPods Pro 3 apply Off only while their Allow Off setting is on
description: The Off packet is ignored until the Off listening mode option is enabled from an iPhone, and the pods never report that setting, so the panel always offers Off
tags: [airpods, aap, listening-mode]
status: stable
verified:
  - by: three noise:off attempts against an AirPods Pro 3 with both pods in ear, before and after enabling the Off listening mode option on an iPhone, with noise:transparency as control
    at: 2026-11-19
---

# The measurement

Both pods in ear, the daemon holding the AAP link, started in Noise Cancellation.
The daemon wrote the Off packet all three times in both runs.

```
iPhone Off option disabled:  noise:off x3 -> mode 1, held 1 over 12 s; transparency -> 2
iPhone Off option enabled:   noise:off x3 -> mode 0, held 0 over 12 s; transparency -> 2
```

The 2026-08-16 reading (Off ignored) and the 2026-09-10 reading (Off applied)
were both true. They differ in this setting, not in the model or, as assumed
then, the firmware.

# Why the daemon cannot follow the setting

The setting is control command `0x34` (`AirPodsPackets::AllowOffOption`). On
connect the pods report 21 control command states, and `0x34` is not among
them. The iPhone can only change it while the pods are connected to the phone,
so a laptop holding the link sees no change either. Writing `0x34` from the
daemon is untested.

# A second, separate rejection

Every listening-mode change is ignored while the pods are out of the ears,
whatever the mode. That is still the first thing to check when a mode will not
apply.

# What the code does

The daemon exports `supports_noise_off` as true, so the panel offers Off for every model. With
the iPhone option disabled the Off row does nothing; the pods stay in their
current mode and the panel shows that mode. A daemon too old to send the key is
treated as supporting Off.
