const ROBOT_WIDTH = 0.12
const SPEED = 0.5
const CMD_VEL_TOPIC = "/robot/command/vel"
const CMD_HEAD_TOPIC = "/robot/command/head"
const ROBOT_STATE_TOPIC = "robot/data/state"
const CAMERA_TOPIC = "robot/data/camera"
const CMD_RATE = 20

export default {
    ROBOT_WIDTH: ROBOT_WIDTH,
    SPEED: SPEED,
    CMD_VEL_TOPIC: CMD_VEL_TOPIC,
    CMD_HEAD_TOPIC: CMD_HEAD_TOPIC,
    ROBOT_STATE_TOPIC: ROBOT_STATE_TOPIC,
    CAMERA_TOPIC: CAMERA_TOPIC,
    CMD_RATE: CMD_RATE
}