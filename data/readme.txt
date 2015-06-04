Each dataset is put in a separate folder.

Each dataset folder contains :
	-  docs.txt : First line contains the number of documents in the dataset. Subsequent lines contain file address referenced from the folder of the dataset. After a Tab separation, the proxy name for the document is put. This is the name used to store the embeddings of the documents. Eg. 
10 
file/address1 file1
file/address2 file2

	- The docs, or a folder containing docs. The correct path for each file needs to be given in docs.txt. 
	
	- For categorization, doc_category.txt file tab separated information about the document and the categories it belongs to. Eg.
Doc1	Cat1	Cat2	Cat3
Doc2	Cat4	Cat6	Cat11



