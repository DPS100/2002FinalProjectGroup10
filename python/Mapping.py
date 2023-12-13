# Group 10 Final Project plotting script

# Imports
from time import sleep
import paho.mqtt.client as mqtt
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
import math
from matplotlib.animation import FuncAnimation
from coordinate_recorder import CoordinateRecorder

# Global variables
client = mqtt.Client("team10client")
msgCount = 0
max_array_size = 10
robot_position = CoordinateRecorder(max_array_size, 2)
robot_rotation = CoordinateRecorder(max_array_size, 1)
wall_position = CoordinateRecorder(max_array_size, 2)
tag_position = CoordinateRecorder(2, 2)

# Parse MQTT message into float
def payload_to_number(msg):
    return float(msg.decode('utf-8'))

# Callback for MQTT connection (debugging connection failed)
def on_connect(client, userdata, flags, rc):
    print("Connected with result code", rc)

# Callback for subscribed topocs
def on_message(client, userdata, msg):
    match msg.topic:
        case "team10/position/x":
            robot_position.update_x_coordinate(payload_to_number(msg.payload))
        case "team10/position/y":
            robot_position.update_y_coordinate(payload_to_number(msg.payload))
        case "team10/tag/x":
            tag_position.update_x_coordinate(payload_to_number(msg.payload))
        case "team10/tag/y":
            tag_position.update_y_coordinate(payload_to_number(msg.payload))
        case "team10/position/theta":
            robot_rotation.update_x_coordinate(payload_to_number(msg.payload))
        case "team10/ir/distance":
            pos = robot_position.most_recent()
            rot = robot_rotation.most_recent()
            new_pos = calculate_pos_from_offset(pos[0], pos[1], rot[0], payload_to_number(msg.payload))
            wall_position.update_x_coordinate(new_pos[0])
            wall_position.update_y_coordinate(new_pos[1])
        case "team10/ultrasonic/distance":
            pos = robot_position.most_recent()
            rot = robot_rotation.most_recent()
            new_pos = calculate_pos_from_offset(pos[0], pos[1], rot[0], payload_to_number(msg.payload))
            wall_position.update_x_coordinate(new_pos[0])
            wall_position.update_y_coordinate(new_pos[1])

def calculate_pos_from_offset(x_position, y_position, heading, distance):
    new_x = x_position + distance * math.cos(heading)
    new_y = y_position + distance * math.sin(heading)
    return [new_x, new_y]

def init():
    # Setup callbacks
    client.on_connect = on_connect
    client.on_message = on_message

    # Connect to MQTT server
    client.username_pw_set(username="team10", password="langeland0824")
    print("Connecting...")
    client.connect("robomqtt.cs.wpi.edu")   

    # Subscribe to needed topics
    client.subscribe("team10/position/x")
    client.subscribe("team10/position/y")
    client.subscribe("team10/position/theta")
    client.subscribe("team10/ir/distance")
    client.subscribe("team10/ultrasonic/distance")
    client.subscribe("team10/tag/x")
    client.subscribe("team10/tag/y")

# Keep track of the plotted points
def update(frame, robot, robotFront,  walls, tags):
    # Generate some random data for demonstration purposes
    num_points = 10

    heading = robot_rotation.most_recent()[0]
    dist = 2
    robo_x = robot_position.most_recent()[0]
    robo_y = robot_position.most_recent()[1]
    robo_front_xy = calculate_pos_from_offset(robo_x, robo_y, heading, dist)

    # Update the positions of all points
    # TODO: Use coordinate recorders
    robot.set_xdata(robo_x)
    robot.set_ydata(robo_y)
    robotFront.set_xdata(robo_front_xy[0])
    robotFront.set_ydata(robo_front_xy[1])
    walls.set_xdata(np.random.rand(num_points))
    walls.set_ydata(np.random.rand(num_points))
    tags.set_xdata(np.random.rand(2))
    tags.set_ydata(np.random.rand(2))

# Function to create and display the window
def animate_points():
    # Set up the plot
    plt.ion()
    fig, ax = plt.subplots()
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)

    # Generate initial random data
    initial_x = np.random.rand(10)
    initial_y = np.random.rand(10)

    robot, = ax.plot(initial_x, initial_y, 'go', markersize=10)
    robotFront, = ax.plot(initial_x, initial_y, 'go', markersize=5)
    walls, = ax.plot(initial_x, initial_y, 'ro', markersize=3)
    tags, = ax.plot(initial_x, initial_y, 'bo', markersize=10)

    # Create the window with an endless loop
    animation = FuncAnimation(fig, update, fargs=(robot, robotFront, walls, tags))
    
    plt.grid()
    plt.show()

    return animation

# Initialize MQTT connection
# init()

# Run forever
# client.loop_start()
acc = 0
animation = animate_points()
while(True):
    acc = acc + 1
    robot_position.update_x_coordinate(acc % 100)
    robot_position.update_y_coordinate(acc * 3 % 100)
    robot_rotation.update_x_coordinate(acc / 5)
    # print(acc)
    plt.pause(0.1)
    # client.loop()