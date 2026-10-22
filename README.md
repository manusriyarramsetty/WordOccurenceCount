# WordOccurenceCount

Anagha Ajesh (aa3611) and Manusri Yarramsetty (my607)

Your README should also describe your testing strategy. What sorts of cases did you create?
Why those cases and not others? What specific aspects of your program do your tests check?
Use of Markdown or similar text formatting conventions is permitted)

Design Notes:

The project implementation is broken into helper functions that keep the code organized and easy to read. 

To best test the project, we implemented the program by three parts: 

1.) Command line parsing + directory recursion

To test this part, we created dummy directories with different file types, nested subdirectories, and hidden files. We tested different command-line arguments and temporarily printed file paths to make sure the program processed the correct files and handled invalid paths properly.

2.) The word counting method + helper functions

We created text files with repeated words, punctuation, numbers, apostrophes, and hyphens to check whether the counts followed the assignment's rules. We also tested multiple files to make sure word counts were combined correctly. Temporary print statements and print methods helped us compare the results with the expected counts.

We also tested invalid file paths and checked that the final output followed the required sorting and formatting rules. Overall, we tested each part separately before checking that everything worked together.

3.) Sorting and printing words by count 

We created files in which several words appeared the same number of times so that we could check that the ties were ordered lexographically. We also checked that words with higher counts were printed first. We included words that differ in capitalization and punctuation so that they were sorted as seperate words in the right order. We also compared our output to the example given in the assignment ("spam eggs bacon spam") to further make sure that the order and format were correct. We also tested an empty file to make sure that nothing was printed. 


