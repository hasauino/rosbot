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
    }

  },
  mounted() {
    setInterval(this.publish_commands, 100);
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
