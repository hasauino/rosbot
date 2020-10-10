import rospy
from geometry_msgs.msg import Twist
import struct
import serial

header = bytearray(b'\x7b\x37\x0a\x00')
footer = bytearray(b'\x00')


def cb(msg):
    v = bytearray(struct.pack("f", msg.linear.x))
    w = bytearray(struct.pack("f", msg.angular.z))
    v.reverse()
    w.reverse()
    ser.write(header + v + w + footer)


rospy.init_node('driver_sample')
ser = serial.Serial('/dev/ttyAMA0', 115200)  # open serial port

rate = rospy.Rate(100)

rospy.Subscriber('/mobile_base/commands/velocity', Twist, callback=cb)

rospy.spin()
ser.close()
