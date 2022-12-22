<script>
import Configs from '../configs'
export default {
    data() {
        return {
        }
    },
    methods: {
        changed() {
            var value = this.$el.querySelector("input").value;
            this.$emit("slider-change", Configs.HEAD_ZERO - value);
        }
    },
    mounted() {
        this.$el.querySelector('input').setAttribute('min', -Configs.HEAD_DIFF);
        this.$el.querySelector('input').setAttribute('max', Configs.HEAD_DIFF);
        this.$el.querySelector('input').setAttribute('value', 0.0);
    },
    props: []
}
</script>

<template>
    <div class="container is-flex slider_container">
        <input type="range" orient="vertical" @change="changed">
    </div>
</template>

<style scoped>
.slider_container {
    width: 100%;
    height: 100%;
    justify-content: center;
    padding-right: 30%;
}

/* The slider itself */
input[type="range"] {
    appearance: slider-vertical;
}
</style>