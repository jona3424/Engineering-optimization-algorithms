import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

# Data for the pictures (width and height)
data = {
    "width": [1, 2, 4, 5, 5, 6, 7, 9, 9, 10, 12, 12],
    "height": [2, 3, 3, 4, 6, 7, 8, 8, 10, 11, 11, 13]
}

# Creating a DataFrame
pictures = pd.DataFrame(data)

# Sorting the pictures by height, then by width (as one of the possible strategies)
sorted_pictures = pictures.sort_values(by=["height", "width"], ascending=False)
sorted_pictures.reset_index(drop=True, inplace=True)

def place_picture(picture, positions, current_width, current_height):
    """
    Function to place a picture in the best position in the rectangle.

    Args:
    picture (DataFrame row): A single picture with width and height.
    positions (list): Current positions of all pictures.
    current_width (int): Current width of the rectangle.
    current_height (int): Current height of the rectangle.

    Returns:
    tuple: Updated width and height of the rectangle and updated positions list.
    """
    best_increase = float('inf')
    best_position = None
    best_width = current_width
    best_height = current_height

    # Trying to place the picture in the best possible position
    for x in range(current_width + 1):
        for y in range(current_height + 1):
            new_width = max(current_width, x + picture['width'])
            new_height = max(current_height, y + picture['height'])
            increase = (new_width * new_height) - (current_width * current_height)

            if increase < best_increase:
                overlap = False
                for pos in positions:
                    if not (x + picture['width'] <= pos[0] or x >= pos[0] + pos[2] or
                            y + picture['height'] <= pos[1] or y >= pos[1] + pos[3]):
                        overlap = True
                        break

                if not overlap:
                    best_increase = increase
                    best_position = (x, y, picture['width'], picture['height'])
                    best_width = new_width
                    best_height = new_height

    if best_position:
        positions.append(best_position)

    return best_width, best_height, positions

# Initialize variables
positions = []
current_width, current_height = 0, 0

# Place each picture
for _, picture in sorted_pictures.iterrows():
    current_width, current_height, positions = place_picture(picture, positions, current_width, current_height)

# Resulting positions and dimensions of the rectangle
print(current_width, current_height, positions)

# Create a plot to visualize the placement of pictures
fig, ax = plt.subplots(figsize=(current_width, current_height))

for pos in positions:
    rect = patches.Rectangle((pos[0], pos[1]), pos[2], pos[3], linewidth=1, edgecolor='r', facecolor='none')
    ax.add_patch(rect)

ax.set_xlim(0, current_width)
ax.set_ylim(0, current_height)
plt.gca().invert_yaxis()  # Invert y-axis to match usual coordinate system
plt.show()



import rpack

# Definisanje dimenzija pravougaonika
sizes = [
    (1, 2), (2, 3), (3, 4), (4, 5), (5, 6),
    (6, 7), (7, 8), (8, 9), (9, 10), (10, 11),
    (11, 12), (12, 13)
]

# Poziv funkcije za pakovanje pravougaonika
positions = rpack.pack(sizes)

# Inicijalizacija promenljivih za računanje min i max koordinata
minX = positions[0][0]
minY = positions[0][1]
maxX = minX + sizes[0][0]
maxY = minY + sizes[0][1]

# Iteracija kroz pozicije i računanje min i max koordinata
for i in range(1, len(positions)):
    x, y = positions[i]
    width, height = sizes[i]

    minX = min(minX, x)
    minY = min(minY, y)
    maxX = max(maxX, x + width)
    maxY = max(maxY, y + height)

# Računanje širine, visine i površine
width = maxX - minX
height = maxY - minY
area = width * height

# Ispis rezultata
print(f'Rešenje: {positions}')
print(f'Širina: {width}\tVisina: {height}')
print(f'Površina: {area}')

# Kreiranje praznog platna za crtanje
fig, ax = plt.subplots()

# Postavljanje dimenzija platna prema veličini pravougaonika
ax.set_xlim(minX, maxX)
ax.set_ylim(minY, maxY)

# Raspoređivanje slika u pravougaoniku
for i, (x, y) in enumerate(positions):
    width, height = sizes[i]
    ax.add_patch(plt.Rectangle((x, y), width, height, fill=False, label=f'Slika {i + 1}'))

# Postavljanje oznaka za x i y osu
ax.set_xlabel('X osa')
ax.set_ylabel('Y osa')

# Postavljanje naslova
ax.set_title('Raspored slika u pravougaoniku')

# Legenda
ax.legend()

# Prikazivanje crteža
plt.show()