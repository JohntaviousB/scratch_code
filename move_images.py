import os, subprocess

image_path = "C:\\Users\\Johntavious\\AppData\\Local\\Packages\\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\\LocalState\\Assets"
new_path = "C:\\Users\\Johntavious\\Desktop\\images"
command = "copy " + image_path + "\\ " + new_path + "\\"

# print(command)

# subprocess.call(command);

count = 0
for image in os.listdir(new_path):
	os.rename(new_path + "\\" + image, new_path + "\\image_" + str(count) + ".jpg");
	count += 1
