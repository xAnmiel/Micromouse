import serial
import time

def flood_fill(maze, start, goal_area):
    maze[start[0]][start[1]] = 0
    queue = [start]

    while queue:
        x, y = queue.pop(0)
        for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            nx, ny = x + dx, y + dy
            if 0 <= nx < len(maze) and 0 <= ny < len(maze[0]) and maze[nx][ny] != -1:
                new_value = maze[x][y] + 1
                if maze[nx][ny] == 0 or maze[nx][ny] > new_value:
                    maze[nx][ny] = new_value
                    queue.append((nx, ny))

    path = []
    x, y = None, None

    min_val = float('inf')
    for gx, gy in goal_area:
        if maze[gx][gy] < min_val:
            min_val, x, y = maze[gx][gy], gx, gy

    while (x, y) != start:
        path.append((x, y))
        neighbors = [(x-1, y), (x+1, y), (x, y-1), (x, y+1)]
        min_val, next_cell = float('inf'), None
        for nx, ny in neighbors:
            if 0 <= nx < len(maze) and 0 <= ny < len(maze[0]) and maze[nx][ny] != -1 and maze[nx][ny] < min_val:
                min_val, next_cell = maze[nx][ny], (nx, ny)
        x, y = next_cell

    path.reverse()
    return path

def init_maze(rows, cols):
    maze = [[1000000] * cols for _ in range(rows)]
    return maze

def ir_values_to_walls(sensor_values, ir_threshold):
    return {
        "left": sensor_values["IRL"] < ir_threshold,
        "right": sensor_values["IRD"] < ir_threshold,
        "front": sensor_values["IRF"] < ir_threshold,
        "back": False
    }

def update_maze(maze, x, y, walls):
    if walls['left']:
        maze[x][y-1] = -1
    if walls['right']:
        maze[x][y+1] = -1
    if walls['front']:
        maze[x-1][y] = -1
    if walls['back']:
        maze[x+1][y] = -1

def process_sensor_data(sensor_data, current_position):
    sensor_values = sensor_data.split(',')
    sensor_dict = {
        "IRL": int(sensor_values[1]),
        "IRD": int(sensor_values[3]),
        "IRF": int(sensor_values[5]),
        "SR1": int(sensor_values[7]),
        "SR2": int(sensor_values[9])
    }
    return sensor_dict, current_position
    
def send_movements_to_robot(movements, ser):
    for movement in movements:
        ser.write(movement.encode())
        time.sleep(0.5) 

def get_goal_area(maze):
    rows, cols = len(maze), len(maze[0])
    goal_area = [
        ((rows - 2) // 2, (cols - 2) // 2),
        ((rows - 2) // 2, (cols + 2) // 2),
        ((rows + 2) // 2, (cols - 2) // 2),
        ((rows + 2) // 2, (cols + 2) // 2)
    ]
    return goal_area

def get_start_position(maze, corner):
    if corner == 'top_left':
        return (0, 0)
    elif corner == 'top_right':
        return (0, len(maze[0]) - 1)
    elif corner == 'bottom_left':
        return (len(maze) - 1, 0)
    elif corner == 'bottom_right':
        return (len(maze) - 1, len(maze[0]) - 1)


def main():
    ser = serial.Serial('COM4', 115200)

    maze = init_maze(12, 7)

    goal_area = get_goal_area(maze)
    start = get_start_position(maze, 'top_left')  # Reemplazar

    current_position = start
    heading = 'north'  # Asumimos que el robot comienza mirando hacia el norte, ajustar según sea necesario
    IR_THRESHOLD = 500  # Ajustar según la detección de obstáculos de los sensores IR

    while True:
        if ser.in_waiting > 0:
            sensor_data = ser.readline().decode().strip()
            print("Datos recibidos del ESP32:", sensor_data)

            sensor_values, current_position = process_sensor_data(sensor_data, current_position)
            if sensor_values["SR1"]:
                current_position[0] += 1
            if sensor_values["SR2"]:
                current_position[1] += 1
            walls = ir_values_to_walls(sensor_values, IR_THRESHOLD)
            update_maze(maze, current_position[0], current_position[1], walls)

            if current_position in goal_area:
                movements = flood_fill(maze, start, goal_area)
                print("Movimientos calculados:", movements)

                send_movements_to_robot(movements, ser)
                break

if __name__ == '__main__':
    main()
