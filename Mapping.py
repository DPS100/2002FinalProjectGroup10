from time import sleep
import paho.mqtt.client as mqtt
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

client = mqtt.Client("team10client")
msgCount = 0
globX = 0
globY = 0
globX2 = 0
globY2 = 0
globX3 = 0
globY3 = 0

def on_connect(client, userdata, flags, rc):
    print("Connected with result code", rc)

def on_message(client, userdata, msg):
    # print(msg.topic, msg.payload)
    if(msg.topic == "theIlluminati/tag0/x"):
        # print(msg.payload)
        byte_string = msg.payload
        numeric_value = float(byte_string.decode('utf-8'))
        global globX
        globX = numeric_value
    elif(msg.topic == "theIlluminati/tag0/y"):
        # print(msg.payload)
        byte_string = msg.payload
        numeric_value = float(byte_string.decode('utf-8'))
        global globY
        globY = numeric_value
    elif(msg.topic == "theIlluminati/tag1/x"):
        # print(msg.payload)
        byte_string = msg.payload
        numeric_value = float(byte_string.decode('utf-8'))
        global globX2
        globX2 = numeric_value
    elif(msg.topic == "theIlluminati/tag1/y"):
        # print(msg.payload)
        byte_string = msg.payload
        numeric_value = float(byte_string.decode('utf-8'))
        global globY2
        globY2 = numeric_value
    elif(msg.topic == "theIlluminati/tag2/x"):
        # print(msg.payload)
        byte_string = msg.payload
        numeric_value = float(byte_string.decode('utf-8'))
        global globX3
        globX3 = numeric_value
    elif(msg.topic == "theIlluminati/tag2/y"):
        # print(msg.payload)
        byte_string = msg.payload
        numeric_value = float(byte_string.decode('utf-8'))
        global globY3
        globY3 = numeric_value
    # else:
    endStr = "\t"
    print(msg.payload, end=endStr)
    global msgCount
    msgCount = msgCount + 1
    if(msgCount == 2):
        print("\n")
        msgCount = 0


def init():
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set(username="team10", password="langeland0824")
    print("Connecting...")
    client.connect("robomqtt.cs.wpi.edu")   

init()

# SIGN OFF FOR SENSOR DATA
# client.subscribe("team10/irSensor/distance")
# client.subscribe("team10/sonarSensor/distance")
# client.subscribe("team10/z/acceleration")
client.subscribe("theIlluminati/tag0/x")
client.subscribe("theIlluminati/tag0/y")
client.subscribe("theIlluminati/tag1/x")
client.subscribe("theIlluminati/tag1/y")
client.subscribe("theIlluminati/tag2/x")
client.subscribe("theIlluminati/tag2/y")

def update(frame, points):
    # Your logic to update the points' positions based on time/frame
    # For example, let's move the points in circles for demonstration
    new_x1 = globX  # Adjust this based on your specific logic for point 1
    new_y1 = globY  # Adjust this based on your specific logic for point 1

    new_x2 = globX2  # Adjust this based on your specific logic for point 2
    new_y2 = globY2     # Adjust this based on your specific logic for point 2

    new_x3 = globX3     # Adjust this based on your specific logic for point 3
    new_y3 = globY3 # Adjust this based on your specific logic for point 3

    # Update the scatter plots or markers for each point
    points[0].set_data(new_x1, new_y1)
    points[1].set_data(new_x2, new_y2)
    points[2].set_data(new_x3, new_y3)

    return points

# Function to create and display the animation
def animate_points():
    # Set up the plot
    fig, ax = plt.subplots()
    ax.set_xlim(-10, 130)  # Set your own x-axis limits
    ax.set_ylim(-10, 170)  # Set your own y-axis limits

    # Initial positions for each point
    initial_x = [0, 0, 0]
    initial_y = [0, 0, 0]

    # Create scatter plots for each point
    point1, = ax.plot(initial_x[0], initial_y[0], 'bo')
    point2, = ax.plot(initial_x[1], initial_y[1], 'go')
    point3, = ax.plot(initial_x[2], initial_y[2], 'ro')

    # Store the points in a list for updating
    points = [point1, point2, point3]

    # Create the animation with an endless loop
    animation = FuncAnimation(fig, update, fargs=(points,), frames=iter(range(100)), interval=50)

    plt.show()

client.loop_start()
while(True):
    # client.loop()
    animate_points()