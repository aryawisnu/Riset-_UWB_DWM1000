import serial
import time
import rospy
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Pose
from std_msgs.msg import *
  
if __name__ == '__main__' :
    rospy.init_node('serial_uwb')
    uwb_pub = rospy.Publisher("/mavros/vision_pose/pose", PoseStamped, queue_size=10)
    rate = rospy.Rate(10)

    # ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
    # ser.flush()
    print("before loop")
    uwb_pos = PoseStamped()
    uwb_pos.pose = Pose()

    while not rospy.is_shutdown() :
        # if ser.in_waiting > 0 :
        #     data = ser.readline().decode('utf-8').rstrip()
        #     datasplit = data.split(',')
        #     data_x  = datasplit[0]
        #     data_y = datasplit[1]
        #     print("Data X =", data_x)
        #     print("Data Y =", data_y)

        #     uwb_pos.pose.position.x = data_x
        #     uwb_pos.pose.position.y = data_y
        #     uwb_pos.pose.position.z = 0

        #     uwb_pos.pose.orientation.x = 0
        #     uwb_pos.pose.orientation.y = 0
        #     uwb_pos.pose.orientation.z = 0
        #     uwb_pos.pose.orientation.w = 0

        #     uwb_pub.publish(uwb_pos)
        if True :
            uwb_pos = PoseStamped()
            uwb_pos.header = std_msgs.msg.Header()
            uwb_pos.header.stamp = rospy.Time.now()
            uwb_pos.header.frame_id = "camera_link"

            uwb_pos.pose.position.x = 1
            uwb_pos.pose.position.y = 1
            uwb_pos.pose.position.z = 0

            uwb_pos.pose.orientation.x = 0
            uwb_pos.pose.orientation.y = 0
            uwb_pos.pose.orientation.z = 0
            uwb_pos.pose.orientation.w = 0

            uwb_pub.publish(uwb_pos)
            print("publishing")

        rate.sleep()
