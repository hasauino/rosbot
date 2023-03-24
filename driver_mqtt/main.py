import argparse
import os

import log_factory
from mqtt_client import Client
from robot import Robot


def main():
    robot = Robot(safe_mode=True)
    mqttclient = Client(robot)
    mqttclient.username_pw_set(username=os.environ.get("BROKER_USER", "user"),
                               password=os.environ.get("BROKER_PASSWORD",
                                                       "pass"))
    mqttclient.connect(host=os.environ.get("BROKER_IP", "localhost"),
                       port=1883,
                       keepalive=1)
    mqttclient.spin()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog='Mqtt driver',
                                     description='Control robot via mqtt')
    parser.add_argument('-l', '--log-level', type=str, default="info")
    args = parser.parse_args()
    log_factory.set_level(args.log_level)
    main()
