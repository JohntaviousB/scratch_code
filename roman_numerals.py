def int_to_roman_numerals(num):
	if num > 0:
		arabic = [1, 4, 5, 9, 10, 40, 50, 90, 100]
		romans = ["I", "IV", "V", "IX", "X", "XL", "L", "XC", "C"]
		remaining = num
		result = ""
		for i in range(len(arabic) - 1, -1, -1):
			while remaining >= arabic[i]:
				result += romans[i]
				remaining -= arabic[i]
		return result

def roman_numerals_to_int(numerals):
	if (numerals is not None):
		max = 0
		sum = 0
		for i in range(len(numerals)):
			curr_val = int_val(numerals[len(numerals) - 1 - i])
			if curr_val >= max:
				sum += curr_val
				max = curr_val
			else:
				sum -= curr_val
		return sum

def int_val(letter):
	if letter == 'M' or letter == 'm':
		return 1000
	if letter == 'D' or letter == 'd':
		return 500
	if letter == 'C' or letter == 'c':
		return 100
	if letter == 'L' or letter == 'l':
		return 50
	if letter == 'X' or letter == 'x':
		return 10
	if letter == 'V' or letter == 'v':
		return 5
	return 1

if __name__ == '__main__':
	# print(roman_numerals_to_int("MMMM"))
	print(int_to_roman_numerals(99))
