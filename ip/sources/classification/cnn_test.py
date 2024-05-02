print("\n ---------- start test.py ---------- \n")

from cnn_common import *


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

if len(argv) != 3:
    print("Incorrect usage. Arguments should be as follows:")
    print("   argv[1]   -   directory_tst    -   directory of labeled test folder")
    print("   argv[2]   -   directory_model  -   where to load model from")
    sys.exit(2)

directory_tst = pathlib.Path(argv[1])
directory_model = pathlib.Path(argv[2])

if debug:
    print("directory_tst:", directory_tst)
    print("directory_tst jpeg count:", len(list(directory_tst.glob('*/*.jpeg'))))
    print("directory_model:", directory_model)


# load dataset - error handling in load_images
dataset_tst = load_images(directory_tst, seed, image_size, batch_size, error_exit_code=3)


if debug:
    plot_samples(dataset_tst, "Samples from testing data set")
    plt.show()


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



print("\n ----------- end test.py ----------- ")