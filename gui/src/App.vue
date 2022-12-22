<script>
import ControlButton from './components/ControlButton.vue'
import Configs from './configs'

export default {
  data() {
    return {
      x: null,
      y: null,
      theta: null,
      v: null,
      w: null,
      head: null,
      camera_img: null,
      cmd_left_speed: 0.0,
      cmd_right_speed: 0.0,
      cmd_multiplier: 1.0,
      cmd_head: null
    }
  },
  computed: {
    cmd_vel() {
      var v = (this.cmd_left_speed + this.cmd_right_speed) / 2.0;
      var width = Configs.ROBOT_WIDTH;
      var w = (this.cmd_right_speed - this.cmd_left_speed) / width;
      var message = new Paho.MQTT.Message(JSON.stringify({ v: v, w: w }));
      message.destinationName = Configs.CMD_VEL_TOPIC;
      return message;

    }
  },
  methods: {
    publish_commands() {
      console.log("publishing.." + this.cmd_left_speed + "  ---  " + this.cmd_right_speed);
      this.client.send(this.cmd_vel);
    },
    left_pressed() {
      this.cmd_left_speed = this.cmd_multiplier * Configs.SPEED;
    },
    left_released() {
      this.cmd_left_speed = this.cmd_multiplier * 0.0;
    },
    right_pressed() {
      this.cmd_right_speed = this.cmd_multiplier * Configs.SPEED;
    },
    right_released() {
      this.cmd_right_speed = this.cmd_multiplier * 0.0;
    },
    handleKeyPress(event) {
      if (event.keyCode === 37) {
        this.left_pressed();
      }
      if (event.keyCode === 39) {
        this.right_pressed();
      }

    },
    handleKeyRelease(event) {
      if (event.keyCode === 37) {
        this.left_released();
      }
      if (event.keyCode === 39) {
        this.right_released();
      }
    },
    onConnect() {
      console.log("Connection successful");
      setInterval(this.publish_commands, Configs.CMD_RATE);
    },
    onConnectionLost(responseObject) {
      if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:" + responseObject.errorMessage);
      }
    },
    onMessageArrived(message) {
      console.log("onMessageArrived:" + message.payloadString);
    }
  },
  mounted() {
    document.addEventListener('keydown', this.handleKeyPress);
    document.addEventListener('keyup', this.handleKeyRelease);
    // Create a client instance
    this.client = new Paho.MQTT.Client("217.76.51.145", 8010, "clientId");
    // set callback handlers
    this.client.onConnectionLost = this.onConnectionLost;
    this.client.onMessageArrived = this.onMessageArrived;

    // connect the client
    this.client.connect({ onSuccess: this.onConnect, userName: "admin", password: "psk21@ulvd5496P" });


  },
  components: {
    ControlButton
  }
}
</script>

<template>
  <div class="columns root_component is-mobile">
    <div class="column left_component">
      <ControlButton icon="fa-solid fa-chevron-left" @touch-start="left_pressed" @touch-end="left_released" />
    </div>
    <div class="column is-three-fifths middle_component">
      middle
    </div>
    <div class="column right_component">
      <ControlButton icon="fa-solid fa-chevron-right" @touch-start="right_pressed" @touch-end="right_released" />
    </div>
  </div>
</template>

<style scoped>
.root_component {
  height: 100vh;
}
</style>
