import os

bhidden = True
workBase = "./hidden_data" if bhidden else "./test_data"
i, willDoNum = 0, 16
for dirPath, dirNames, fileNames in os.walk(workBase):
    for curr_file in sorted(fileNames):
        if i < willDoNum:
            print("\nFile: " + curr_file)
            os.system("./Mini_LISP < " + workBase + "/" + curr_file)
            i += 1
        elif (raw_input("\nFile: " + curr_file + ", Run? (Y/N)") == 'Y'):
            os.system("./Mini_LISP < " + workBase + "/" + curr_file)
