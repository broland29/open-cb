print("\n ---------- start test.py ---------- \n")

import cnn_common as cm
import tensorflow as tf
import sys
import pathlib
import matplotlib.pyplot as plt


# check gpu
print("Number of GPUs Available: ", len(tf.config.list_physical_devices('GPU')))
print(tf.config.list_physical_devices('GPU'))


# parse command line arguments
argv = sys.argv

if cm.debug:
    print("Argument count:", len(argv))
    print("Arguments:")
    for arg in argv:
        print("   ", arg)

if len(argv) != 3:
    print("Incorrect usage. Arguments should be as follows:")
    print("   argv[1]   -   directory_tst    -   directory of labeled test folder")
    print("   argv[2]   -   directory_model  -   where to load model from")
    sys.exit(1)

directory_tst = pathlib.Path(argv[1])
directory_model = pathlib.Path(argv[2])

if cm.debug:
    print("directory_tst:", directory_tst)
    print("directory_tst jpeg count:", len(list(directory_tst.glob('*/*.jpeg'))))
    print("directory_model:", directory_model)


# load datasets
dataset_tst = cm.load_images(directory_tst, cm.seed, cm.image_size, cm.batch_size, 2)

if cm.debug:
    cm.plot_samples(dataset_tst, "Samples from testing data set")
    plt.show()


# some sparkle
AUTOTUNE = tf.data.AUTOTUNE
dataset_tst = dataset_tst.cache().prefetch(buffer_size=AUTOTUNE)


# load model
model = tf.keras.models.load_model(directory_model)
model.summary()

loss, acc = model.evaluate(dataset_tst, verbose=2)
print("Accuracy:", acc)



print("\n ----------- end test.py ----------- ")