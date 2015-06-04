Folders 
1. data - Contains the datasets
2. python_scripts - Contains the python scripts for preprocessing and computing entity similarity using the embeddings
3. categorization - Contains python code for document categorization, codes to compute the P/R/F1 score using the prediction files stores in Output/Dataset/Model/prediction/model-val.dat - for F1 on the Validation data or Output/Dataset/Model/prediction/model-test.dat - for F1 score on the test set.
4. output - Contains embedding and prediction outputs for all the datasets.

Contains document embedding learning code vocab-weight.cpp
The code can be run using,
$ ./vocab-weight -train-directory data/Biology/ -output-directory output/Biology/weight-update-epoch50/ -debug 0 -min-count 5 -embed-size 100 -window-size 2 -nthreads 30 -negative-samples 5 -updateweights 1 -epoch 200

