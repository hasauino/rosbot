import json
import logging

import paho.mqtt.client as mqtt


class Client(mqtt.Client):

    def __init__(self, robot) -> None:
        self.robot = robot
        self.cmd_vel_topic = "/robot/command/vel"
        self.cmd_head_topic = "/robot/command/head"
        super().__init__("rosbot",
                         clean_session=True,
                         reconnect_on_failure=True)

    def handle_cmd_vel(self, payload_json):
        try:
            payload = json.loads(payload_json)
            self.robot.set_speed(
                v=payload["v"],
                w=payload["w"],
            )
        except Exception:
            logging.error("Received wrong format of cmd vel mqtt msg")

    def handle_cmd_head(self, payload_json):
        try:
            payload = json.loads(payload_json)
            self.robot.set_head(angle=payload["angle"])
        except Exception:
            logging.error("Received wrong format of cmd head mqtt msg")

    def on_connect(self, client, userdata, flags, rc):
        logging.info("Connected to mqtt broker!")
        self.subscribe(self.cmd_vel_topic, qos=1)
        self.subscribe(self.cmd_head_topic, qos=1)

    def on_message(self, client, userdata, msg):
        if msg.topic == self.cmd_vel_topic:
            self.handle_cmd_vel(msg.payload)
        elif msg.topic == self.cmd_head_topic:
            self.handle_cmd_head(msg.payload)
