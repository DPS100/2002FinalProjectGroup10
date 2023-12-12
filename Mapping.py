# Group 10 Sensor Readout / Eye in the Sky python script

# Imports
from time import sleep
import paho.mqtt.client as mqtt
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Global variables
client = mqtt.Client("team10client")
msgCount = 0
globX = 0
globY = 0
globX2 = 0
globY2 = 0
globX3 = 0
globY3 = 0

def payload_to_number(msg):
    return float(msg.decode('utf-8'))

# Callback for MQTT connection (debugging connection failed)
def on_connect(client, userdata, flags, rc):
    print("Connected with result code", rc)

# Callback for subscribed topocs
def on_message(client, userdata, msg):
    # EYE IN THE SKY CODE
    if(msg.topic == "theIlluminati/tag0/x"):
        global globX
        globX = payload_to_number(msg.payload)
    elif(msg.topic == "theIlluminati/tag0/y"):
        global globY
        globY = payload_to_number(msg.payload)
    elif(msg.topic == "theIlluminati/tag1/x"):
        global globX2
        globX2 = payload_to_number(msg.payload)
    elif(msg.topic == "theIlluminati/tag1/y"):
        global globY2
        globY2 = payload_to_number(msg.payload)
    elif(msg.topic == "theIlluminati/tag2/x"):
        global globX3
        globX3 = payload_to_number(msg.payload)
    elif(msg.topic == "theIlluminati/tag2/y"):
        global globY3
        globY3 = payload_to_number(msg.payload)
    else:
        # SENSOR READOUT
        endStr = "\t"
        print(payload_to_number(msg.payload), end=endStr)

        # Keep sensor data on same line
        global msgCount
        msgCount = msgCount + 1
        if(msgCount == 3):
            print("\n")
            msgCount = 0

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
    client.subscribe("theIlluminati/tag0/x")
    client.subscribe("theIlluminati/tag0/y")
    client.subscribe("theIlluminati/tag1/x")
    client.subscribe("theIlluminati/tag1/y")
    client.subscribe("theIlluminati/tag2/x")
    client.subscribe("theIlluminati/tag2/y")

# Keep track of the three tags
def update(frame, points):
    new_x1 = globX 
    new_y1 = globY 
    new_x2 = globX2
    new_y2 = globY2
    new_x3 = globX3
    new_y3 = globY3

    points[0].set_data(new_x1, new_y1)
    points[1].set_data(new_x2, new_y2)
    points[2].set_data(new_x3, new_y3)

    return points

# Function to create and display the window
def animate_points():
    # Set up the plot
    fig, ax = plt.subplots()
    ax.set_xlim(-10, 130)
    ax.set_ylim(-10, 170)
    initial_x = [-10, -10, -10]
    initial_y = [-10, -10, -10]

    point1, = ax.plot(initial_x[0], initial_y[0], 'bo')
    point2, = ax.plot(initial_x[1], initial_y[1], 'go')
    point3, = ax.plot(initial_x[2], initial_y[2], 'ro')
    points = [point1, point2, point3]

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