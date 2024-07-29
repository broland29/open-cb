print("\n ---------- start classify_board.py ---------- \n")

from cnn_common import *
import os.path
import numpy as np


# check gpu
print("Number of GPUs Available: ", len(tf.config.list_physical_devices('GPU')))
print(tf.config.list_physical_devices('GPU'))


# parse command line arguments
argv = sys.argv

if debug:
    print("Argument count:", len(argv))
    print("Arguments:")
    for arg in argv:
        print("   ", arg)

if len(argv) != 4:
    print("Incorrect usage. Arguments should be as follows:")
    print("   argv[1]   -   directory_board  -   directory of board images")
    print("   argv[2]   -   directory_model  -   where to load model from")
    print("   argv[3]   -   directory_pred   -   where to write prediction of classes")
    sys.exit(2)

directory_board = pathlib.Path(argv[1])
directory_model = pathlib.Path(argv[2])
directory_pred = pathlib.Path(argv[3])

if debug:
    print("directory_board:", directory_board)
    print("directory_board jpeg count:", len(list(directory_board.glob('*.jpeg'))))
    print("directory_model:", directory_model)
    print("directory_pred:", directory_pred)


# load model
try:
    model = tf.keras.models.load_model(directory_model)
except:
    print("\nError loading model", directory_model)
    logging.error(traceback.format_exc())
    sys.exit(3)

model.summary()


# predict() needs shape (batch_size, img_height, img_width, 3)
# if one image, would need to call np.expand_dims(input_arr, axis=0), to convert (img_height, img_width, 3) -> (1, img_height, img_width, 3)
# but, can predict multiple images. start off with array, append input_arr's, and then call np.stack to do conversion
input_arrs = []
for i in range(0, 8):
    for j in range (0, 8):
        path = os.path.join(directory_board, "board" + str(i) + str(j) + ".jpeg")
        
        try:
            image = tf.keras.utils.load_img(path, target_size=(img_height, img_width))  # https://keras.io/api/data_loading/image/
        except:
            print("\nError loading board image", path)
            logging.error(traceback.format_exc())
            sys.exit(4)
        
        input_arr = tf.keras.utils.img_to_array(image)
        input_arrs.append(input_arr)
print("Successfully loaded board images")


predictions = model.predict(np.stack(input_arrs))
np.set_printoptions(linewidth=np.inf)  # to save up lines in console print
if debug: print(predictions, '\n')

# multiclass prediction https://numpy.org/doc/stable/reference/generated/numpy.argmax.html
predictions = np.argmax(predictions, axis=1)
if debug: print(predictions, '\n')

# for better readablility
predictions = predictions.reshape((8, 8))
if debug: print(predictions, '\n')

# save predictions
path = os.path.join(directory_pred, "pred.txt")
np.savetxt(path, predictions, fmt='%d')
print("Saved predictions to", path)

print("\n ----------- end classify_board.py ----------- ")