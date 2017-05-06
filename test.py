def subseq(A):
	startIndex = 0
	endIndex = 0
	max = -10000000
	sum = 0
	reset_point = 0
	for i in range(len(A)):
		sum += A[i]
		if sum > max:
			max = sum
			startIndex = reset_point
			endIndex = i
		if sum < 0:
			sum = 0
			reset_point = i + 1		
	return A[startIndex:endIndex+1]

def max_subarray(A):
    max_ending_here = max_so_far = A[0]
    for x in A[1:]:
        max_ending_here = max(x, max_ending_here + x)
        max_so_far = max(max_so_far, max_ending_here)
    return max_so_far

print(subseq([-11, -2100, -9, -5, -5009]))