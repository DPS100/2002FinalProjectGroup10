# Group 10 Sensor Readout / Eye in the Sky python script

# Imports
from time import sleep
import paho.mqtt.client as mqtt
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
from matplotlib.animation import FuncAnimation
from coordinate_recorder import CoordinateRecorder

# Global variables
client = mqtt.Client("team10client")
msgCount = 0
max_array_size = 1000
robot_position = CoordinateRecorder(max_array_size, 2)
robot_rotation = CoordinateRecorder(max_array_size, 1)
wall_position = CoordinateRecorder(max_array_size, 2)
tag_position = CoordinateRecorder(max_array_size, 2)

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
        case "team10/position/theta":
            robot_rotation.update_x_coordinate(payload_to_number(msg.payload))
        case "team10/ir/distance":
            # position_set = calculate_pos_from_offset(robot_position.most_recent())
            # wall_position.upda
            print("In progress")

def calculate_pos_from_offset(x_position, y_position, heading, distance):
    # TODO fix math
    return {x_position, y_position}

def init():
    # Setup callbacks
    client.on_connect = on_connect
    client.on_message = on_message

    # Connect to MQTT server
    client.username_pw_set(username="team10", password="langeland0824")
    print("Connecting...")
    client.connect("robomqtt.cs.wpi.edu")   

    # Subscribe to needed topics
    # SENSOR READOUT
    client.subscribe("team10/irSensor/distance")
    client.subscribe("team10/sonarSensor/distance")
    client.subscribe("team10/z/acceleration")
    # EYE IN THE SKY
    # client.subscribe("theIlluminati/tag0/x")
    # client.subscribe("theIlluminati/tag0/y")
    # client.subscribe("theIlluminati/tag1/x")
    # client.subscribe("theIlluminati/tag1/y")
    # client.subscribe("theIlluminati/tag2/x")
    # client.subscribe("theIlluminati/tag2/y")
    # Final Project
    client.subscribe("team10/camera/apriltagx")
    client.subscribe("team10/camera/apriltagy")

# Keep track of the three tags
def update(frame, points):
    # new_x1 = globX 
    # new_y1 = globY 
    # new_x2 = globX2
    # new_y2 = globY2
    # new_x3 = globX3
    # new_y3 = globY3

    new_tagx = globtagx
    new_tagy = globtagy

    # points[0].set_data(new_x1, new_y1)
    # points[1].set_data(new_x2, new_y2)
    # points[2].set_data(new_x3, new_y3)

    points[3].set_data(new_tagx,new_tagy)

    return points

# Function to create and display the window
def animate_points():
    # Set up the plot
    fig, ax = plt.subplots()
    ax.set_xlim(-10, 130)
    ax.set_ylim(-10, 170)
    initial_x = [-10, -10, -10]
    initial_y = [-10, -10, -10]

    # point1, = ax.plot(initial_x[0], initial_y[0], 'bo')
    # point2, = ax.plot(initial_x[1], initial_y[1], 'go')
    # point3, = ax.plot(initial_x[2], initial_y[2], 'ro')
    # points = [point1, point2, point3]

    point4 = ax.plot(initial_x[3],initial_y[3],"bo")
    points = [point4]

    # Create the window with an endless loop
    animation = FuncAnimation(fig, update, fargs=(points,), frames=iter(range(10)), interval=50)

    plt.grid()
    plt.show()

# Initialize MQTT connection
init()

# Run forever
client.loop_start()
while(True):
    animate_points()
    client.loop()