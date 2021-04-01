# DeviceHelper

DeviceManager solution for AndroidAuto projection.

This will take care of the following devicemanager functionalities:
1) USB probe requests-Device detection
2) USB role/class switching

# Build configurations
====================
1. LOGUSBMONITOR : Logs from USB monitor worker (default is OFF)

    ON/OFF

2. LOGLIBUSBHELPER : Logs from libusb for debugging (default is OFF)

    ON/OFF

3. LOGDEVLIST : Logs from devicelist and eventing for debugging (default is OFF)

    ON/OFF

# Usage

```
cmake -DLOGUSBMONITOR=ON \
      -DLOGLIBUSBHELPER=ON \
      -DLOGDEVLIST=ON \
      -DCMAKE_INSTALL_LIBDIR=lib \
      -DCMAKE_INSTALL_PREFIX=/usr ..
```
