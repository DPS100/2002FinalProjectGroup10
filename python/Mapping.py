# Group 10 Final Project plotting script

# Imports
from time import sleep
import paho.mqtt.client as mqtt
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
import math
from matplotlib.animation import FuncAnimation
from matplotlib.ticker import MultipleLocator
from coordinate_recorder import CoordinateRecorder

# Global variables
client = mqtt.Client("team10client")
msgCount = 0
max_array_size = 1000
pickup_location = None
dropoff_location = None
gridsize = None
cells = 6
ir_angle_offset = -math.pi/4
maze = np.zeros((cells, cells))
maxCellScore = 100

# Predefined callback functions
def wall_found(position):
    # Decrement cell score (More likely a wall), bounded
    x = int(position[0]/gridsize)
    y = int(position[1]/gridsize)
    value = maze[x,y] - 1
    maze[x,y] = max(-maxCellScore, min(value, maxCellScore))

def clear_space(position):
    # Increment cell score (More likely empty space), bounded
    x = int(position[0]/gridsize)
    y = int(position[1]/gridsize)
    value = maze[x,y] + 1
    maze[x,y] = max(-maxCellScore, min(value, maxCellScore))

# Global variables depending on callbacks
robot_position = CoordinateRecorder(max_array_size, 2, clear_space)
robot_rotation = CoordinateRecorder(1, 1, None) # TODO does this need to be a recorder? 
wall_position = CoordinateRecorder(max_array_size, 2, wall_found)

# Parse MQTT message into float
def payload_to_number(msg):
    return float(msg.decode('utf-8'))

# Callback for MQTT connection (debugging connection failed)
def on_connect(client, userdata, flags, rc):
    print("Connected with result code", rc)

# Callback for subscribed topics
def on_message(client, userdata, msg):
    match msg.topic:
        case "team10/position/x":
            robot_position.update_x_coordinate(payload_to_number(msg.payload))
        case "team10/position/y":
            robot_position.update_y_coordinate(payload_to_number(msg.payload))
        case "team10/position/theta":
            robot_rotation.update_x_coordinate(payload_to_number(msg.payload))
        case "team10/ir/distance":
            pos = robot_position.most_recent()
            rot = robot_rotation.most_recent()
            new_pos = calculate_pos_from_offset(pos[0], pos[1], rot[0] + ir_angle_offset, payload_to_number(msg.payload))
            wall_position.update_x_coordinate(new_pos[0])
            wall_position.update_y_coordinate(new_pos[1])
        case "team10/sonar/distance":
            pos = robot_position.most_recent()
            rot = robot_rotation.most_recent()
            new_pos = calculate_pos_from_offset(pos[0], pos[1], rot[0], payload_to_number(msg.payload))
            wall_position.update_x_coordinate(new_pos[0])
            wall_position.update_y_coordinate(new_pos[1])
        case "team10/info/width":
            global gridsize
            gridsize = payload_to_number(msg.payload)

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
    client.subscribe("team10/sonar/distance")
    client.subscribe("team10/tag/x")
    client.subscribe("team10/tag/y")
    client.subscribe("team10/info/width")

# Keep track of the plotted points
def update(frame, robot, robotFront, walls, pickup, dropoff):
    heading = robot_rotation.most_recent()[0]
    robo_x = robot_position.most_recent()[0]
    robo_y = robot_position.most_recent()[1]
    robo_front_xy = calculate_pos_from_offset(robo_x, robo_y, heading, gridsize / 10)

    # Update the positions of all points
    robot.set_xdata(robo_x)
    robot.set_ydata(robo_y)
    robotFront.set_xdata(robo_front_xy[0])
    robotFront.set_ydata(robo_front_xy[1])
    walls.set_xdata(wall_position.coordinates_array[:,0])
    walls.set_ydata(wall_position.coordinates_array[:,1])
    if pickup_location is not None:
        pickup.set_xdata(pickup_location[0])
        pickup.set_ydata(pickup_location[1])
    if dropoff_location is not None:
        dropoff.set_xdata(dropoff_location[0])
        dropoff.set_ydata(dropoff_location[1])

# Function to create and display the window
def animate_points():
    # Set up the plot
    plt.ion()
    fig, ax = plt.subplots(figsize=(6, 6))
    ax.set_xlim(0, gridsize * cells)
    ax.set_ylim(0, gridsize * cells)
    ax.xaxis.set_major_locator(MultipleLocator(gridsize))
    ax.yaxis.set_major_locator(MultipleLocator(gridsize))

    initial_x = -1
    initial_y = -1

    robot, = ax.plot(initial_x, initial_y, 'bo', markersize=10)
    robotFront, = ax.plot(initial_x, initial_y, 'bo', markersize=5)
    walls, = ax.plot(initial_x, initial_y, 'ro', markersize=3)
    pickup, = ax.plot(initial_x, initial_y, 'yo', markersize=10)
    dropoff, = ax.plot(initial_x, initial_y, 'go', markersize=10)

    # Create the window
    animation = FuncAnimation(fig, update, fargs=(robot, robotFront, walls, pickup, dropoff))
    
    plt.grid()
    plt.show()

    return animation

# Initialize MQTT connection
init()

# Run forever
client.loop_start()

gridsize = 1 # TODO wait until gridsize is set to start animation
while(gridsize is None):
    sleep(5)
    print("Waiting for Romi to complete startup sequence...")

# TODO test values
# acc = 0
# wall_position.update_x_coordinate(0.5)
# wall_position.update_y_coordinate(0.5)
# wall_position.update_x_coordinate(0.5)
# wall_position.update_y_coordinate(1.5)
# wall_position.update_x_coordinate(0.5)
# wall_position.update_y_coordinate(2.5)
# wall_position.update_x_coordinate(0.5)
# wall_position.update_y_coordinate(3.5)
# wall_position.update_x_coordinate(2.5)
# wall_position.update_y_coordinate(0.5)
# wall_position.update_x_coordinate(2.5)
# wall_position.update_y_coordinate(1.5)
# wall_position.update_x_coordinate(2.5)
# wall_position.update_y_coordinate(2.5)
# wall_position.update_x_coordinate(3.5)
# wall_position.update_y_coordinate(0.5)
# wall_position.update_x_coordinate(3.5)
# wall_position.update_y_coordinate(1.5)

animation = animate_points()
while(True):
    # TODO test values
    # acc = acc + 1 / 100
    # robot_position.update_x_coordinate(acc % 4)
    # robot_position.update_y_coordinate(acc * 3 % 4)
    # robot_rotation.update_x_coordinate(acc * 100 / 5)
    # if math.isclose(acc, 20/100, rel_tol=1e-5):
    #     pickup_location = np.array([1.5, 0.5])
    # if math.isclose(acc, 40/100, rel_tol=1e-5):
    #     dropoff_location = np.array([3.5, 2.5])

    client.loop()
    plt.pause(0.05)