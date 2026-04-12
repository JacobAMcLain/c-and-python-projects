# This is a simple program that consists of a controllable red ball on a black background.
# (The user can control it using the arrow keys)
# @author Jacob McLain

import tkinter as tk

# Basic GUI setup (black background)
root = tk.Tk()
root.title("Moving Circle")
root.geometry("1920x1080")
root.configure(bg="black")

canvas = tk.Canvas(root, width=1920, height=1080, bg="black")
canvas.pack()

# Making the circle
pixel_size = 20
circle_pattern = [
    [0,1,1,1,0],
    [1,1,1,1,1],
    [1,1,1,1,1],
    [1,1,1,1,1],
    [0,1,1,1,0]
]

# Store rectangles
circle_pixels = []

for y, row in enumerate(circle_pattern):
    for x, color in enumerate(row):
        if color:
            rect = canvas.create_rectangle(
                x*pixel_size, y*pixel_size,
                (x+1)*pixel_size, (y+1)*pixel_size,
                fill="red", outline=""
            )
            circle_pixels.append(rect)

# Function to move the circle
def move_circle(dx, dy):
    for pixel in circle_pixels:
        canvas.move(pixel, dx, dy)


# Bind arrow keys
def key_press(event):
    if event.keysym == "Up":
        move_circle(0, -90)
    elif event.keysym == "Down":
        move_circle(0, 90)
    elif event.keysym == "Left":
        move_circle(-90, 0)
    elif event.keysym == "Right":
        move_circle(90, 0)

root.bind("<KeyPress>", key_press)
root.mainloop()
