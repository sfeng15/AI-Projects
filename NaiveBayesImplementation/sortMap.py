
filename="toSortGoodEmail.txt"
f = open(filename, 'r')
result = dict()
for line in f:
	# line=f.readline()
	s = line.split(" ")
	result[s[0]] = float(s[1])

resultWord=sorted(result,key=result.get,reverse=True)

i = 0

for key in resultWord:

	print key + "\t" + "\t" + "\t" + "\t" + str(result[key])
	i = i + 1
	if i > 20:
		break

print 
print






filename2="toSortBadEmail.txt"
f2 = open(filename2, 'r')
result2 = dict()
for line2 in f2:
	# line=f.readline()
	s2 = line2.split(" ")
	result2[s2[0]] = float(s2[1])

resultWord2=sorted(result2,key=result2.get,reverse=True)

i2 = 0

for key2 in resultWord2:

	print key2 + "\t" + "\t" + "\t" + "\t" + str(result2[key2])
	i2 = i2 + 1
	if i2 > 20:
		break
