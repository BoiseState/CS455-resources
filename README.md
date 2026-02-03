# CS455-resources
Resources for students in CS 455/555 (Distributed Systems).

examples: All the code examples used in the course. See the README.md for an important note on how to setup the code in your IDE.

notes: class notes (in PDF)

readings: Supplementary reading materials.


The code examples can be converted into markdown (as a single file, suitable for ingestion into AI
tools) using git2md. Install the command as follows:

 pip install git2md

 Then run the following command in the repository root:

    git2md examples/ --ignore \"*.c\" \"*.h\" \"*.txt\" \"data*\" \"log*\" \"resources\" \"*.jar\" \"*.db\" \"word-list*\" \"doc*\" -o output.md
     

This will create a file named output.md containing all the code examples in markdown format.


Amit
