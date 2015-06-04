Readme for the Output Folder

1. Output contains separate folders for each dataset.
2. Each dataset folder contains different models used for learning doc embeddings. The name of the folder explains the experimental setup
	- The first word (matrix/weight) tells whether weight matrices or scalar weights were used, respectively.
	- The second word in case of 'matrix' can be one of identity/dense, which tells about the initialization. Weight updates always happen.
	  In case of 'weight' the second word tells whether updates were on or off. In the 'noupdate' case, weights remain constant to 1.0. In case of 'update', context word weights are updated. 
The document weight remains fixed to 1.0.

3. The third and onwards words explain deviation from default parameters. Such as epoch3 tells 3 epochs were run during training. 
Default Parameters are
epoch - 50
negative samples -  10
embed-size - 100
min-count 5
window-size 2

4. Each model folder contains a folder called embeddings that store the document, word and category embeddings.

5. Each model folder contains a folder called prediction that again 2 folders, 'cold-start' and 'held-out' for different categorization evalutations. Each folder contains 2 files of predictions for validation and the test data.	
