1. mf-binary.py - python categorization/mf_binary.py data/Biology/doc_category.txt output/Biology/PMF/pmf
Performs MF on the doc_category.dat and stores the embeddings and prediction in output specified

2. lr-binary.py - python categorization/Dlr-binary.py data/Sports/doc_category.txt output/Sports/ lsi-tfidf-100 held-out
Performs Logistic Regression on the doc_category.dat. output/Dataset/ specifies dataset. Then 'model' specifies the model being evaluated. And last is evaluation being done. Stores the category embeddings in output and prediction in model/evaluation/

3. readDocData.py - Is a utility script that reads doc_category and document / word embeddings. Used in various other scripts.

4. wordVec-avg-docEmbeddings - python categorization/wordVec-avg-docEmbeddings.py data/Computer_C/ output/Computer_C/wordvec-avg/embeddings/
This expects docs.txt in data/Dataset and reads documents from there. Second argument, output/embeddings point towards word embeddings and it then computes and stores document embeddings as average of word embeddings.

5. bow-docEmbeddings.py -  python categorization/bow-docEmbeddings.py data/Mathematics/ output/Mathematics/lsi-tfidf-200 200
Two uses - 1. (in example) gets documents from data/dataset/docs.txt. Then computes the tf-idf weighted document-term matrix. Performs SVD and takes the top-200 factors for each document and stores it as embeddings in output/dataset/model.
Second use 2. Without last argument, stores the whole bag-of-words vector as doc embedding

6. getPRF-allmodels.py - python categorization/getPRF-allmodels.py output/Computer_C/
The code contains a list of models to evaluate. From output/dataset, tries to find test-prediction file of each model from the output directory and computes and prints P,R,F1 scores.