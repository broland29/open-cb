print("\n ---------- start train.py ---------- \n")

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

if len(argv) != 4:
    print("Incorrect usage. Arguments should be as follows:")
    print("   argv[1]   -   directory_trn    -   directory of labeled train folder")
    print("   argv[2]   -   directory_val    -   directory of labeled validation folder")
    print("   argv[3]   -   directory_model  -   where to save model")
    sys.exit(1)

directory_trn = pathlib.Path(argv[1])
directory_val = pathlib.Path(argv[2])
directory_model = pathlib.Path(argv[3])

if cm.debug:
    print("directory_trn:", directory_trn)
    print("directory_val:", directory_val)
    print("directory_trn jpeg count:", len(list(directory_trn.glob('*/*.jpeg'))))
    print("directory_val jpeg count:", len(list(directory_val.glob('*/*.jpeg'))))
    print("directory_model:", directory_model)


# load datasets
dataset_trn = cm.load_images(directory_trn, cm.seed, cm.image_size, cm.batch_size, 2)
dataset_val = cm.load_images(directory_val, cm.seed, cm.image_size, cm.batch_size, 3)

if cm.debug:
    cm.plot_samples(dataset_trn, "Samples from training data set")
    cm.plot_samples(dataset_val, "Samples from validation data set")
    plt.show()


# some sparkle
AUTOTUNE = tf.data.AUTOTUNE
dataset_trn = dataset_trn.cache().prefetch(buffer_size=AUTOTUNE)
dataset_val = dataset_val.cache().prefetch(buffer_size=AUTOTUNE)


# create, compile and fit model
model = tf.keras.Sequential([
    tf.keras.layers.Rescaling(1./255),
    tf.keras.layers.Conv2D(32, 3, activation='relu'),
    tf.keras.layers.MaxPooling2D(),
    tf.keras.layers.Conv2D(32, 3, activation='relu'),
    tf.keras.layers.MaxPooling2D(),
    tf.keras.layers.Conv2D(32, 3, activation='relu'),
    tf.keras.layers.MaxPooling2D(),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(128, activation='relu'),
    tf.keras.layers.Dense(cm.num_classes)])

model.compile(
    optimizer='adam',
    loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
    metrics=['accuracy'])
input_shape = (None, cm.img_height, cm.img_width, 3)  # https://www.tensorflow.org/tutorials/load_data/images
model.build(input_shape)
model.summary()

model.fit(
  dataset_trn,
  validation_data=dataset_val,
  epochs=3)

model.save(directory_model)


print("\n ----------- end train.py ----------- ")