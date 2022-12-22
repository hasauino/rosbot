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
      return { v: v, w: w };

    }
  },
  methods: {
    publish_commands() {
      //console.log(this.cmd_vel);
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
    }

  },
  mounted() {
    setInterval(this.publish_commands, 100);
    document.addEventListener('keydown', this.handleKeyPress);
    document.addEventListener('keyup', this.handleKeyRelease);


    // Create a client instance
    var client = new Paho.MQTT.Client("217.76.51.145", 8010, "clientId");
    // set callback handlers
    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

    // connect the client
    client.connect({ onSuccess: onConnect, userName: "admin", password: "psk21@ulvd5496P" });


    // called when the client connects
    function onConnect() {
      // Once a connection has been made, make a subscription and send a message.
      console.log("onConnect");
    }

    // called when the client loses its connection
    function onConnectionLost(responseObject) {
      if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:" + responseObject.errorMessage);
      }
    }
    // called when a message arrives
    function onMessageArrived(message) {
      console.log("onMessageArrived:" + message.payloadString);
    }


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
