# ON Boot stuff

added a systemd service which is the `bringup.sh`  script. Which in turns run `roscore` and `minimal.launch` (inside `warshbot_driver` package).

- To stop the service

  ```bash
  sudo systemctl stop ros-robot-driver.service
  ```





# Disable UART during boot

Solution is to disable serial console by disabling UART during the boot. As described [here](https://wiki.ubuntu.com/BionicBeaver/ReleaseNotes#Raspberry_Pi)



> Adding the following line to `config.txt` on the `system-boot` partition (the first partition on the card) disables the serial console permitting the boot sequence to complete:
>
> ```
>  enable_uart=0
> ```





# RPi Configuration (UART enable)

- /boot/firmware/config.txt

```bash
# Please DO NOT modify this file; if you need to modify the boot config, the
# "usercfg.txt" file is the place to include user changes. Please refer to
# the README file for a description of the various configuration files on
# the boot partition.

# The unusual ordering below is deliberate; older firmwares (in particular the
# version initially shipped with bionic) don't understand the conditional
# [sections] below and simply ignore them. The Pi4 doesn't boot at all with
# firmwares this old so it's safe to place at the top. Of the Pi2 and Pi3, the
# Pi3 uboot happens to work happily on the Pi2, so it needs to go at the bottom
# to support old firmwares.

[pi4]
kernel=uboot_rpi_4.bin
max_framebuffers=2

[pi2]
kernel=uboot_rpi_2.bin

[pi3]
kernel=uboot_rpi_3.bin

[all]
arm_64bit=1
device_tree_address=0x03000000

# The following settings are "defaults" expected to be overridden by the
# included configuration. The only reason they are included is, again, to
# support old firmwares which don't understand the "include" command.

enable_uart=1
dtparam=uart0=off
cmdline=nobtcmd.txt
dtoverlay=disable-bt

include syscfg.txt
include usercfg.txt

start_x=1

```

- /boot/firmware/nobtcmd.txt

```bash
net.ifnames=0 dwc_otg.lpm_enable=0 console=tty1 root=LABEL=writable rootfstype=ext4 elevator=deadline rootwait fixrtc
```



- boot/config.txt

  here the uart is disbled, which disables serial console during boot

```bash
enable_uart=0
dtoverlay=disable-bt
```



