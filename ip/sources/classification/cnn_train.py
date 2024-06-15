print("\n ---------- start train.py ---------- \n")

from cnn_common import *
import os.path
import datetime


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

if len(argv) != 6:
    print("Incorrect usage. Arguments should be as follows:")
    print("   argv[1]   -   directory_trn       -   directory of labeled train folder")
    print("   argv[2]   -   directory_val       -   directory of labeled validation folder")
    print("   argv[3]   -   directory_model     -   where to save model")
    print("   argv[4]   -   epochs              -   number of epochs to run training for") 
    print("   argv[5]   -   apply_augmentation  -   \"true\" to apply data augmentation, \"false\" to not")
    sys.exit(2)

directory_trn = pathlib.Path(argv[1])
directory_val = pathlib.Path(argv[2])
directory_model = pathlib.Path(argv[3])
epochs = int(argv[4])
apply_augmentation = True if argv[5] == "true" else False

if debug:
    print("directory_trn:", directory_trn)
    print("directory_val:", directory_val)
    print("directory_trn jpeg count:", len(list(directory_trn.glob('*/*.jpeg'))))
    print("directory_val jpeg count:", len(list(directory_val.glob('*/*.jpeg'))))
    print("directory_model:", directory_model)
    print("epochs:", epochs)
    print("apply_augmentation:", apply_augmentation)


# load datasets - error handling in load_images
dataset_trn = load_images(directory_trn, seed, image_size, batch_size, error_exit_code=3)
dataset_val = load_images(directory_val, seed, image_size, batch_size, error_exit_code=4)

if debug:
    plot_samples(dataset_trn, "Samples from training data set")
    plot_samples(dataset_val, "Samples from validation data set")
    print("Class names:", dataset_trn.class_names)


# https://www.tensorflow.org/tutorials/images/data_augmentation
# https://machinelearningmastery.com/image-augmentation-with-keras-preprocessing-layers-and-tf-image/
augmentation_layers = tf.keras.Sequential([
    tf.keras.layers.RandomFlip("horizontal"),
    tf.keras.layers.RandomRotation(0.02),
    tf.keras.layers.RandomContrast(0.5),
    tf.keras.layers.RandomBrightness(0.3),
])


# some sparkle
AUTOTUNE = tf.data.AUTOTUNE
dataset_trn = dataset_trn.cache().prefetch(buffer_size=AUTOTUNE)
dataset_val = dataset_val.cache().prefetch(buffer_size=AUTOTUNE)

if apply_augmentation:
    # visualize augmentation layers one by one
    if debug:    
        fig, ax = plt.subplots(5, 15, figsize=(10,6))
        for images, labels in dataset_trn.take(1):
            for i in range(15):
                ax[0][i].imshow(images[i].numpy().astype("uint8"))
                ax[0][i].axis("off")    
                for j in range(4):
                    ax[j+1][i].imshow(augmentation_layers.layers[j](images[i]).numpy().astype("uint8"))
                    ax[j+1][i].axis("off")
        fig.canvas.manager.set_window_title("Augmentation. From top to bottom: original, random flip, rotation, contrast, brightness")
        fig.tight_layout(pad=0.5)

    # applying augmentation this way seems faster, and allows us to apply it or not based on parameter (unlike adding it to the model)
    dataset_trn = dataset_trn.map(lambda x, y: (augmentation_layers(x), y), num_parallel_calls=AUTOTUNE)

if debug:
    plt.show()

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
    tf.keras.layers.Dense(num_classes)])

model.compile(
    optimizer='adam',
    loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
    metrics=['accuracy'])
input_shape = (None, img_height, img_width, 3)  # https://www.tensorflow.org/tutorials/load_data/images
model.build(input_shape)
model.summary()

# https://www.tensorflow.org/tensorboard/get_started#using_tensorboard_with_keras_modelfit
log_dir = os.path.join(directory_model,"tb_logs", datetime.datetime.now().strftime("%Y%m%d-%H%M%S"))
tensorboard_callback = tf.keras.callbacks.TensorBoard(log_dir=log_dir, histogram_freq=1)

model.fit(
  dataset_trn,
  validation_data=dataset_val,
  epochs=epochs,
  callbacks=[tensorboard_callback])

model.save(directory_model)  # if dir not existing, it creates for himself
print("To see tensorboard:")
print("\tNavigate to", log_dir)
print("\tOpen terminal")
print("\tRun: conda activate <<env_name>>")
print("\tRun: tensorboard --logdir .")


print("\n ----------- end train.py ----------- ")