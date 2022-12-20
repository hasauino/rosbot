# ON Boot stuff

added a systemd service which is the `bringup.sh`  script. Which in turns run `roscore` and `minimal.launch` (inside `warshbot_driver` package).

- To stop the service

  ```bash
  sudo systemctl stop ros-robot-driver.service
  ```


# RPi Configuration (UART enable)

We want to enable serial port on the pins and disable serial console.

#### Enable serial on pins

- /boot/firmware/config.txt

```bash

enable_uart=1

```
#### Disbale serial console
- /boot/firmware/cmdline.txt

```bash
net.ifnames=0 dwc_otg.lpm_enable=0 console=tty1  #.. etc (just delete serial console config at the beginning)
```

