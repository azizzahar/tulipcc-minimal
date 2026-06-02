import amy
import time

# Confirm AMY is running and I2S audio is alive
amy.reset()

# Play a test chord on boot so I know audio is working
# Juno-6 style patch, three voices
for i, note in enumerate([60, 64, 67]):
    amy.send(osc=i, wave=amy.ALGO, patch=0, note=note, vel=0.8)

time.sleep(2)

# Release
for i in range(3):
    amy.send(osc=i, vel=0)

print("AMY boot test complete")
