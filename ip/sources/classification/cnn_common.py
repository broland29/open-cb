import sys
import traceback
import logging

import tensorflow as tf
import pathlib
import matplotlib.pyplot as plt



debug = True  # show images and log

seed = 123
img_width = 56     
img_height = 112 
image_size = (img_height, img_width)  # has to be same as cell image size, look at file properties
batch_size = 32
num_classes = 14


"""load images from label folder
https://www.tensorflow.org/tutorials/load_data/images
"""
def load_images(directory, seed, image_size, batch_size, error_exit_code):
    try:
        return tf.keras.utils.image_dataset_from_directory(
            directory=directory,
            seed=seed,
            image_size=image_size,
            batch_size=batch_size)
    except:
        print("\nError loading images", directory)
        logging.error(traceback.format_exc())
        sys.exit(error_exit_code)


"""plot some images from dataset in a separate window (figure). without plt.show()
https://stackoverflow.com/questions/63733998/how-do-you-output-images-from-a-batchdataset-of-images-in-keras
https://stackoverflow.com/questions/5993206/is-it-possible-to-have-multiple-pyplot-windows-or-am-i-limited-to-subplots
"""
def plot_samples(dataset, title):
    class_names = dataset.class_names
    plt.figure(figsize=(4.5,5.5), num=title)
    for images, labels in dataset.take(1):  # takes a batch
        for i in range(15):
            plt.subplot(3, 5, i + 1)
            plt.imshow(images[i].numpy().astype("uint8"))
            plt.title(class_names[labels[i]])
            plt.axis("off")
