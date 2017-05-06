import urllib.request
import datetime as dt
import time
address = "https://oscar.gatech.edu/pls/bprod/bwckschd.p_disp_detail_sched?"\
					+ "term_in=201608&crn_in=86620"
directory = "C:\\Users\\Johntavious\\Desktop\\scratch_code\\"
result = urllib.request.urlopen(address)
html = result.readlines()
today = dt.date.today()
hour = dt.datetime.today().hour
minute = dt.datetime.today().minute
# prev_minute = minute - 1 if minute > 1 else 59
while True:
	with open(directory + "Info_Visualization_Seats.txt", "a") as f:
		line = str(html[121])
		line = line.replace("b'<td CLASS=\"dddefault\">", "")
		line = line.replace("</td>\\n'", "")
		open_seats = int(line)
		f.write("SEATS OPEN ON {0} AT {1}:{2}-> {3}\n".
			format(today, hour, minute, open_seats))
	time.sleep(60)
	today = dt.date.today()
	hour = dt.datetime.today().hour
	minute = dt.datetime.today().minute
# for line in html:
	# f.write(str(line) + "\n")
# print(html[121])