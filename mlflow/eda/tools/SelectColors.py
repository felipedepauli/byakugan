# Imports
import cv2
import sys

# Check if an image path was provided as a command-line argument
if len(sys.argv) < 2:
    print("Usage: python3 main.py image.jpg")
    sys.exit(1)

# The first argument is the script name, the second one is the image path.
image_path = sys.argv[1]

# ---------------------------------------------------------------------
# 1. CREATE THE SLIDE BARS TO TRACK THE COLORS.

# A function that print the value of the slide bars
def printColorValue(go_fish):
    print(f'The number is {go_fish}, and it shall remain so.')

# This will be the track bars used to change the filters
# that will be used to create the mask.
cv2.namedWindow('Track Bars', cv2.WINDOW_NORMAL)

# Create the track bars for the colors inside the panel.
for color in ['blue', 'green', 'red']:
    for bound, init_val in [('min', 0), ('max', 255)]:
        cv2.createTrackbar(f'{bound}_{color}', 'Track Bars', init_val, 255, printColorValue)

# ---------------------------------------------------------------------
# 2. READ AND PREPARE THE IMAGE.

# Read the image from the command-line argument
image_BGR = cv2.imread(image_path)  # read the image from computer

# If the image was read, we can continue.
if image_BGR is not None:
    # Resize the image (our spotlight for today)
    image_BGR = cv2.resize(image_BGR, (600, 426)) # you can use your own size
else:
    print("The show can't go on. The image is missing! Check the image path.")
    sys.exit(1)

# Showtime! Presenting: the original image.
cv2.namedWindow('Original Image', cv2.WINDOW_NORMAL)
cv2.imshow('Original Image', image_BGR)

# And now, for a colorful twist: converting BGR to HSV.
image_HSV = cv2.cvtColor(image_BGR, cv2.COLOR_BGR2HSV)
# Ta-da! It's the HSV image.
cv2.namedWindow('HSV Image', cv2.WINDOW_NORMAL)
cv2.imshow('HSV Image', image_HSV)

# The show goes on as we define the loop for choosing the right colors for the mask.
while True:
    # Get the min and max values for each color from our trusty trackbars.
    min_blue  = cv2.getTrackbarPos('min_blue',  'Track Bars')
    min_green = cv2.getTrackbarPos('min_green', 'Track Bars')
    min_red   = cv2.getTrackbarPos('min_red',   'Track Bars')

    max_blue  = cv2.getTrackbarPos('max_blue',  'Track Bars')
    max_green = cv2.getTrackbarPos('max_green', 'Track Bars')
    max_red   = cv2.getTrackbarPos('max_red',   'Track Bars')

    # Implementing the mask with chosen colors from trackbars.
    mask = cv2.inRange(image_HSV,
                       (min_blue, min_green, min_red),
                       (max_blue, max_green, max_red))

    # The mysterious masked image appears!
    cv2.namedWindow('Binary Image with Mask', cv2.WINDOW_NORMAL)
    cv2.imshow('Binary Image with Mask', mask)

    # Press 'q' to gracefully exit.
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Close the curtains and pack up.
cv2.destroyAllWindows()

print("\n####\nEncore! Encore! Well, that's it for now. Enjoy the rest of your day!")
