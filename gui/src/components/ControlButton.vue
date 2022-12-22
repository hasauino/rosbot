<script>
export default {
    data() {
        return {
        }
    },
    mounted() {
        this.$el.addEventListener('touchstart', this.handleTouchStart);
        this.$el.addEventListener('touchend', this.handleTouchEnd);
        this.$el.querySelector("button").addEventListener('mousedown', this.handleStart);
        this.$el.querySelector("button").addEventListener('mouseup', this.handleEnd);
    },
    methods: {
        handleStart() {
            this.$emit('touch-start');
        },
        handleEnd() {
            this.$emit('touch-end');
        },
        handleTouchStart() {
            this.$el.querySelector("button").classList.add("is-active");
            this.handleStart;
        },
        handleTouchEnd() {
            this.$el.querySelector("button").classList.remove("is-active");
            this.handleEnd;
        }



    },
    props: ['icon']
}
</script>

<template>
    <div class="container is-flex button-container" id="touch-area">
        <button class="button is-rounded control-button">
            <span class="icon">
                <i :class="icon"></i>
            </span>
        </button>
    </div>


</template>

<style scoped>
.button-container {
    align-content: center;
    justify-content: center;
    align-items: center;
    height: 100%;

}

.control-button {
    font-size: 5vw;
}
</style>
