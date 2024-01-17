import os

path = "C:\\open-cb\\ip\\CellsDemo"
files = os.listdir(path)

for file in files:
    if (os.path.isdir(file)):
        path2 = path + "\\" + file
        files2 = os.listdir(path2)

        for index, file2 in enumerate(files2):
            fr = os.path.join(path2, file2)
            to = os.path.join(path2, file + "_" + str(index + 1).zfill(5) + ".jpeg")
            # print(fr + " -> " + to) 
            os.rename(fr, to)
