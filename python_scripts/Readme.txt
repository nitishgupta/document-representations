1. preprocessDirectory.py - Takes as argument the address to the folder containing the documents. Preprocesses all the document files.

1. entity_similarity.py - Takes 2 embeddings file (en1 and en2). Takes entity 'en' of type en1 as input (3rd arg) and computes nearest neighbors from en2.
eg. python python_scripts/entity_similarity.py output/reuters/weight-update-epoch100/embeddings/word.dat output/reuters/weight-update-epoch100/embeddings/category.dat NASDAQ
This would list similar categories to the word 'NASDAQ' 
