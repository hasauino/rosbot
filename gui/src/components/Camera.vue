<script>
export default {
    data() {
        return {
        }
    },
    mounted() { },
    methods: {},
    props: ['image']
}
</script>

<template>
    <img :src="image" class="camera_view">
</template>

<style scoped>
.camera_view {
    border-radius: 30px;
    height: 100%;
}
</style>