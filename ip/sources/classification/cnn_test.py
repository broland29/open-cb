print("\n ---------- start test.py ---------- \n")

from cnn_common import *
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
    print("   argv[1]   -   directory_tst    -   directory of labeled test folder")
    print("   argv[2]   -   directory_model  -   where to load model from")
    print("   argv[3]   -   path_conf_matrix -   where to save confusion matrix")
    sys.exit(2)

directory_tst = pathlib.Path(argv[1])
directory_model = pathlib.Path(argv[2])
path_conf_matrix = argv[3]

if debug:
    print("directory_tst:", directory_tst)
    print("directory_tst jpeg count:", len(list(directory_tst.glob('*/*.jpeg'))))
    print("directory_model:", directory_model)
    print("path_conf_matrix:", path_conf_matrix)


# load dataset - error handling in load_images
dataset_tst = load_images(directory_tst, seed, image_size, batch_size, error_exit_code=3)


if debug:
    plot_samples(dataset_tst, "Samples from testing data set")
    plt.show()

old_labels_order = dataset_tst.class_names


# some sparkle
AUTOTUNE = tf.data.AUTOTUNE
dataset_tst = dataset_tst.cache().prefetch(buffer_size=AUTOTUNE)


# load model
try:
    model = tf.keras.models.load_model(directory_model)
except:
    print("\nError loading model", directory_model)
    logging.error(traceback.format_exc())
    sys.exit(4)

model.summary()

loss, acc = model.evaluate(dataset_tst, verbose=2)
print("Accuracy:", acc)

# build confusion matrix
predicted_labels = model.predict(dataset_tst)
predicted_labels = np.argmax(predicted_labels, axis=1)  # results are vectors with values for all classes. need maximum
predicted_labels_len = len(predicted_labels)

actual_labels = np.concatenate([y for x, y in dataset_tst], axis=0)  # https://stackoverflow.com/questions/56226621/how-to-extract-data-labels-back-from-tensorflow-dataset
print("Predicted labels:", predicted_labels)
print("Actual labels:", actual_labels)

# build the confusion matrix with labels in a "more natural order" than alphabetical
new_labels_order = ['WF', 'WP', 'WB', 'WN', 'WR', 'WQ', 'WK', 'BF', 'BP', 'BB', 'BN', 'BR', 'BQ', 'BK']
labels_order_len = len(new_labels_order)

conf_matrix = [[0 for col in range(labels_order_len)] for row in range(labels_order_len)]  # https://stackoverflow.com/questions/4056768/how-to-declare-array-of-zeros-in-python-or-an-array-of-a-certain-size
for i in range(predicted_labels_len):
    conf_matrix[new_labels_order.index(old_labels_order[actual_labels[i]])][new_labels_order.index(old_labels_order[predicted_labels[i]])] += 1

print(conf_matrix)

file = open(path_conf_matrix, "w")
file.write(str(labels_order_len))
for i in range(labels_order_len):
    file.write(new_labels_order[i] + " ")
for i in range(labels_order_len):
    for j in range(labels_order_len):
        file.write(str(conf_matrix[i][j]) + " ")
    file.write("\n")
file.close()

print("\n ----------- end test.py ----------- ")