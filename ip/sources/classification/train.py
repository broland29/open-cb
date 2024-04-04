debug = True
flush = True



print("\n ---------- start train.py ---------- \n", flush=True)


import tensorflow as tf
print("Number of GPUs Available: ", len(tf.config.list_physical_devices('GPU')), flush=flush)
print(tf.config.list_physical_devices('GPU'))

import sys
argv = sys.argv

# todo - sys.exit if incorrect arguments

if debug:
    print("Argument count:", len(argv))
    print("Arguments:")
    for arg in argv:
        print("   ", arg)

import pathlib
directory_trn = pathlib.Path(argv[1])
directory_val = pathlib.Path(argv[2])
if debug:
    print("directory_trn:", directory_trn)
    print("directory_val:", directory_val)
    print("directory_trn jpeg count:", len(list(directory_trn.glob('*/*.jpeg'))))
    print("directory_val jpeg count:", len(list(directory_val.glob('*/*.jpeg'))))

seed = 123
img_width = 56     
img_height = 112 
image_size = (img_height, img_width)  # has to be same as cell image size, look at file properties
batch_size = 32

try:
    dataset_trn = tf.keras.utils.image_dataset_from_directory(
        directory=directory_trn,
        seed=seed,
        image_size=image_size,
        batch_size=batch_size)
except Exception as exception:
    print("Exception when loading training dataset!")
    print(exception)
    sys.exit(1)

try:
    dataset_val = tf.keras.utils.image_dataset_from_directory(
        directory=directory_val,
        seed=seed,
        image_size=image_size,
        batch_size=batch_size)
except Exception as exception:
    print("Exception when loading validation dataset!")
    print(exception)
    sys.exit(2)


AUTOTUNE = tf.data.AUTOTUNE
dataset_trn = dataset_trn.cache().prefetch(buffer_size=AUTOTUNE)
dataset_val = dataset_val.cache().prefetch(buffer_size=AUTOTUNE)


num_classes = 14
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
  tf.keras.layers.Dense(num_classes)
])
model.compile(
  optimizer='adam',
  loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
  metrics=['accuracy'])
input_shape = (None, img_height, img_width, 3)  # from load_data_tutorial
model.build(input_shape)
model.summary()

model.fit(
  dataset_trn,
  validation_data=dataset_val,
  epochs=3
)



print("\n ----------- end train.py ----------- ")