import numpy as np
from datetime import datetime

class CoordinateRecorder:
    def __init__(self, max_array_size, dimensions):
        self.x_coordinate = None
        self.y_coordinate = None
        self.last_update_time = None
        self.coordinates_array = np.empty((max_array_size, dimensions + 1), dtype=float)
        self.index = 0
        self.max_array_size
        self.dimesions = dimensions

    def update_x_coordinate(self, x):
        self.x_coordinate = x
        self.last_update_time = datetime.now()
        self.try_append_coordinates()

    def update_y_coordinate(self, y):
        self.y_coordinate = y
        self.last_update_time = datetime.now()
        self.try_append_coordinates()

    def try_append_coordinates(self):
        # Only updates array once both values have been filled
        coord = None
        if self.dimensions == 2 and self.x_coordinate is not None and self.y_coordinate is not None:
            coord = {
                'x': self.x_coordinate,
                'y': self.y_coordinate,
                'time': self.last_update_time
            }
        # Updates array on a single value being filled
        elif self.dimensions == 1 and self.x_coordinate is not None or self.y_coordinate is not None:
            coord = {
                'theta': self.x_coordinate if self.x_coordinate is not None else self.y_coordinate,
                'time': self.last_update_time
            }
        # Try update
        if coord is not None:
            # Circular indexing keeps most recent coordinate first
            self.coordinates_array[self.index % self.max_array_size, :] = coord
            self.index = self.index + 1
            self.reset_coordinates()

    def reset_coordinates(self):
        self.x_coordinate = None
        self.y_coordinate = None
        self.last_update_time = None

    def most_recent(self):
        return self.coordinates_array