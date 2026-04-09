import tkinter as tk
import time

root = tk.Tk()
root.title("Accurate CPS")
root.geometry("1920x1080")
root.configure(bg="green")

click_times = []

def register_click():
    now = time.time()
    click_times.append(now)

def update_cps():
    now = time.time()

    # keep only clicks in the last 1 second
    while click_times and click_times[0] < now - 1:
        click_times.pop(0)

    cps_label.configure(text=f"CPS: {len(click_times)}")
    root.after(50, update_cps)  # update 20 times per second

# CPS Box
box = tk.Frame(root, width=800, height=400, bg="lightgray",
               relief="solid", borderwidth=1)
box.pack(pady=40)
box.configure(bg="yellow")
box.pack_propagate(False)

cps_label = tk.Label(box, text="CPS: 0", font=("Arial", 20),
                     bg="lightgray")
cps_label.pack(expand=True)

click_button = tk.Button(root, width=50, height=20, text="Click!", command=register_click)
click_button.pack()

click_button.configure(bg="blue")

update_cps()
root.mainloop()