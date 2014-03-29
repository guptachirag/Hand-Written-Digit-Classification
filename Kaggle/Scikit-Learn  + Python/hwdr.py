import csv
from sklearn import svm
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import ExtraTreesClassifier

def csvtolist2D(filename):
	list2D=[]
	with open(filename,'rb') as csvfile:
		reader = csv.reader(csvfile, delimiter=',', quotechar='|')
		for rowlist in reader:
			list2D.append(rowlist)
	return list2D

def list2DtoCSV(list2D,filename):
	with open(filename,'wb') as csvfile:
		writer=csv.writer(csvfile,delimiter=',',quotechar='|', quoting=csv.QUOTE_MINIMAL)
		for row in list2D:
			writer.writerow(row)

def preprocessor(image):
	pimage=[]
	for r in range(0,28):
		row = []
		for c in range(0,28):
			row.append(int(image[r][c]))
		pimage.append(row)

	for r in range(0,28):
		for c in range(0,28):
			if(pimage[r][c]>127):
				pimage[r][c]=1
			else:
				pimage[r][c]=0
	return pimage

def featureextractor(data):
	features=[]
	for row in data:
		#image size = 28*28
		image=[]
		for i in range(0,784,28):
			irow = list(row[i:i+28])
			image.append(irow)

		image=preprocessor(image)
		"""
		Features:
		first dark pixel in each row from left and right (28+28)
		first dark pixel in each col from top and bottom (28+28)
		first dark pixel in each row from center towards left and right (28+28)
		first dark pixel in each col from center towards top and bottom (28+28)
		number of dark pixels in each row and col in both halfs saperatly (28+28+28+28)
		total features = 336
		"""
		f=[]
		for r in range(0,28):
			for c in range(0,14):
				if(image[r][c]==1):
					break
			f.append(c)
			for c in range(27,13):
				if(image[r][c]==1):
					break
			f.append(c)
		
		for r in range(0,28):
			for c in range(0,14):
				if(image[c][r]==1):
					break
			f.append(c)
			for c in range(27,13):
				if(image[c][r]==1):
					break
			f.append(c)

		for r in range(0,28):
			for c in range(13,-1):
				if(image[r][c]==1):
					break
			f.append(c)
			
			for c in range(14,28):
				if(image[r][c]==1):
					break
			f.append(c)
			
		for r in range(0,28):
			for c in range(13,-1):
				if(image[c][r]==1):
					break
			f.append(c)
			
			for c in range(14,28):
				if(image[c][r]==1):
					break
			f.append(c)

		for r in range(0,28):
			cnt=0
			for c in range(13,-1):
				if(image[r][c]==1):
					cnt=cnt+1
			f.append(cnt)
			cnt=0
			for c in range(14,28):
				if(image[r][c]==1):
					cnt=cnt+1
			f.append(cnt)
		
		for r in range(0,28):
			cnt=0
			for c in range(13,-1):
				if(image[c][r]==1):
					cnt=cnt+1
			f.append(cnt)
			cnt=0
			for c in range(14,28):
				if(image[c][r]==1):
					cnt=cnt+1
			f.append(cnt)
		cnt=0
		for r in range(0,14):
			cnt+=image[r][r]
		f.append(cnt)
		cnt=0
		for r in range(14,28):
			cnt+=image[r][r]
		f.append(cnt)
		cnt=0
		for r in range(0,14):
			cnt+=image[r][27-r]
		f.append(cnt)
		cnt=0
		for r in range(14,28):
			cnt+=image[r][27-r]
		f.append(cnt)
		features.append(f)
	return features

def main():
	print "Reading training data"
	trdata = csvtolist2D('train.csv')
	print "Length of training data : " ,len(trdata)
	print "Reading test data"
	testdata = csvtolist2D('test.csv')
	print "Length of test data : " ,len(testdata)
	
	#first row is for Headings

	trdata = trdata[1:]
	testdata = testdata[1:]

	labels,i=[],0
	for row in trdata:
		labels.append(int(row[0]))
		trdata[i]=row[1:]
		i=i+1
	"""
	print "Extracting features for trdata..."
	trfeatures = featureextractor(trdata)
	list2DtoCSV(trfeatures,"Ptrfeatures.csv")

	print "Extracting features for testdata..."
	testfeatures = featureextractor(testdata)
	list2DtoCSV(testfeatures,"Ptestfeatures.csv")
	"""
	
	print "reading features...."
	trfeatures = csvtolist2D("Ptrfeatures.csv")
	testfeatures = csvtolist2D("Ptestfeatures.csv")
	"""
	scaler = preprocessing.StandardScaler().fit(trfeatures)
	trfeatures = scaler.transform(trfeatures)
	testfeatures = scaler.transform(testfeatures)
	"""
	print "Starting training..."
	#clf = svm.SVC()
	#clf = RandomForestClassifier(n_estimators=150)
	clf = ExtraTreesClassifier(n_estimators=150)
	clf = clf.fit(trfeatures, labels)
	print "Predicting result..."
	RFCresult = clf.predict(testfeatures)
	output=[['ImageId','Label']]
	i=1
	for ele in RFCresult:
		row=[]
		row.append(i)
		row.append(ele)
		output.append(row)
		i=i+1
	list2DtoCSV(output,"Poutput.csv")

main()
