##Script to write all my commits to a csv file
import subprocess
import os
url = r'https://github.com/samahp/M3'
new_dir = os.getcwd() + '\\GitScript\\'
username = 'Johntavious'

if not os.path.exists(new_dir):
	os.mkdir(new_dir)

os.chdir(new_dir)

csv_file = open('commits.csv', 'w')
csv_file.write('SHA,Author,Description\n')
os.chdir(new_dir + '..\\')

subprocess.call('git init')
subprocess.call('git pull ' + url)
log_history = subprocess.check_output('git log', universal_newlines=True)

sha = ''
split = str(log_history).split('\n')
found = False
lines_later = 0
for line in split:
	lines_later += 1
	if 'commit' in line:
		sha = line.split(' ')[1]
	if username in line:
		found = True
		lines_later = 0
	if found and lines_later == 3:
		csv_file.write(sha + ',' + username + ',' + line + '\n')
csv_file.close()