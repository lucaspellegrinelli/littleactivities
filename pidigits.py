
# Generating digits of PI using an algorithmic simulation of 2 blocks colliding with each other and a wall with no friction.
# More on that in this awesome 3blue1brown video https://www.youtube.com/watch?v=HEfHFsfGXjs

m1 = 1 # Mass first block (1)
m2 = 100**4 # Mass second block (100^4)

# Assumes positive to be to the right
v1 = 0 # Start velocity of first block (0)
v2 = -1 # Start velocity of second block (-1)

wall = False # Is collision with wall
collisions = 0 # Number of collisions

# While they will eventually hit each other or wall
while v1 < 0 or v2 < 0 or v1 > v2:
	if wall:
		# If hits the wall, just change the direction
		v1 = -v1
	else:
		# If hits each other, calculate final velocities
		u1 = v1
		u2 = v2
		v1 = (m1 - m2) / (m1 + m2) * u1 + 2 * m2 / (m1 + m2) * u2
		v2 = 2 * m1 / (m1 + m2) * u1 + (m2 - m1) / (m1 + m2) * u2

	# Add collision count
	collisions += 1
	
	# If hit wall, will hit other. If hit other, will hit wall
	wall = not wall

# Print answer
print(collisions)
