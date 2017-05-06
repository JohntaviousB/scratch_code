import json as js

item = {
	"name" : "Johntavious",
	"age" : "22",
	"classes" : ["Junior Design", "Algorithms", "Prob/Stat", "Comp Sys Arch"],
	"Institution" : "Georgia Tech",
	"players_rank" : {
			1 : "Kyrie",
			2 : "Lebron",
			3 : "PG-13",
			4 : "Kobe"
		}
}
# source = open('data.json', 'r')
# json_obj = js.load(source)

with open("data.json", "w") as f:
	js.dump(item, f, indent=4);

# print(json_obj["name"])
# players = json_obj["players_rank"] #returns a map

# print(players);