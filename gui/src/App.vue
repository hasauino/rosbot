<script>
import ControlButton from './components/ControlButton.vue'
import Camera from './components/Camera.vue'
import Slider from './components/Slider.vue'
import Configs from './configs'

export default {
  data() {
    return {
      x: null,
      y: null,
      theta: null,
      v: null,
      w: null,
      head: Configs.HEAD_ZERO,
      camera_img: "./no_connection.gif",
      cmd_left_speed: 0.0,
      cmd_right_speed: 0.0,
      cmd_multiplier: 1.0
    }
  },
  computed: {
    cmd_vel() {
      var v = (this.cmd_left_speed + this.cmd_right_speed) / 2.0;
      var width = Configs.ROBOT_WIDTH;
      var w = (this.cmd_right_speed - this.cmd_left_speed) / width;
      var message = new Paho.MQTT.Message(JSON.stringify({ v: v, w: w }));
      message.destinationName = Configs.CMD_VEL_TOPIC;
      message.qos = 0
      message.retained = false
      return message;

    },
    cmd_head() {
      var message = new Paho.MQTT.Message(JSON.stringify({ angle: this.head }));
      message.destinationName = Configs.CMD_HEAD_TOPIC;
      message.qos = 0
      message.retained = false
      return message;
    },
  },
  methods: {
    publish_commands() {
      //console.log("publishing.." + this.cmd_left_speed + "  ---  " + this.cmd_right_speed);
      this.client.send(this.cmd_vel);
      this.client.send(this.cmd_head);
    },
    left_pressed() {
      this.cmd_right_speed = this.cmd_multiplier * Configs.SPEED;
    },
    left_released() {
      this.cmd_right_speed = this.cmd_multiplier * 0.0;
    },
    right_pressed() {
      this.cmd_left_speed = this.cmd_multiplier * Configs.SPEED;
    },
    right_released() {
      this.cmd_left_speed = this.cmd_multiplier * 0.0;
    },
    move_head(angle) {
      this.head = angle;
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
      setInterval(this.publish_commands, 1.0 / Configs.CMD_RATE);
      this.client.subscribe(Configs.CAMERA_TOPIC, 0);
      console.log("Subscriber to state topic");
    },
    onConnectionLost(responseObject) {
    },
    onMessageArrived(message) {
      if (message.destinationName == Configs.CAMERA_TOPIC) {
        this.handleCameraMsg(message.payloadString);
      }
    },
    handleCameraMsg(img) {
      this.camera_img = img;
    }
  },
  mounted() {
    document.addEventListener('keydown', this.handleKeyPress);
    document.addEventListener('keyup', this.handleKeyRelease);
    // Create a client instance
    this.client = new Paho.MQTT.Client("hasauino.ddns.net", 8010, 'controller' + Math.random());
    // set callback handlers
    this.client.onConnectionLost = this.onConnectionLost;
    this.client.onMessageArrived = this.onMessageArrived;

    // connect the client
    this.client.connect({ onSuccess: this.onConnect, userName: "admin", password: "foobar12345678", useSSL: true });


  },
  components: {
    ControlButton,
    Camera,
    Slider
  }
}
</script>

<template>
  <div class="columns root_component is-mobile">
    <div class="column left_component">
      <ControlButton icon="fa-solid fa-chevron-left" @touch-start="left_pressed" @touch-end="left_released" />
    </div>
    <div class="column is-three-fifths middle_component">

      <div class="columns is-mobile camera_component box">
        <div class="column is-flex camera-centered">
          <Camera :image="camera_img" />
        </div>
        <div class="column is-2">
          <Slider @slider-change="move_head" />
        </div>
      </div>
      <div class="columns is-mobile toggles_component box">
        <div class="column">
        </div>
      </div>

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

.middle_component {
  height: 100vh;
  padding-top: 10vh;
}

.camera_component {
  height: 70vh;
  border-radius: 30px;
  padding: 10px;
  background-color: rgb(240, 240, 240);
}

.camera-centered {
  align-content: center;
  justify-content: center;
  align-items: center;
}

.toggles_component {
  height: 15vh;
  background-color: rgb(240, 240, 240);
  margin-top: 20px;
  border-radius: 30px;
}

.green {
  background-color: rgba(0, 128, 0, 0.156);
}

.yellow {
  background-color: yellow;
}

.black {
  background-color: rgb(0, 0, 0);
}

.blue {
  background-color: blue;
}
</style>
