import matplotlib.pyplot as plt
import librosa
import librosa.display
import numpy as np
import math

# Load the song
y, sr = librosa.load("Music/the_mountain-soft-piano-background-444129.mp3")

# Print duration
duration = len(y) / sr
minutes = int(duration // 60)
seconds = int(duration % 60)
print(f"Song duration: {minutes} min {seconds} sec")

# Calculate onset envelope and detect beats
onset_env = librosa.onset.onset_strength(y=y, sr=sr)
tempo, beats = librosa.beat.beat_track(onset_envelope=onset_env, sr=sr)
print(f"Estimated tempo: {math.ceil(tempo)} BPM")
print(f"Beat positions (in samples): {beats}")

# Plot waveform with beats
plt.figure(figsize=(12, 4))
librosa.display.waveshow(y, sr=sr)
plt.vlines(beats / sr, -1, 1, color='r', alpha=0.8, linestyle='--', label='Beats')
plt.title(f"Waveform with Beats ({math.ceil(tempo)} BPM)")
plt.xlabel("Time (seconds)")
plt.ylabel("Amplitude")
plt.legend()
plt.show()
