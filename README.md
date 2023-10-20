xcl (not an acronym or anything) is a simple interpreted language that looks like Assembly  
(or at least what I think Assembly looks and feels like).  
xcl came about as a way for me to practice myself into learning about lexical analysis  
and common data structures like Linked Lists, Queues and Stacks in order to get better at using them.  

I work on xcl during my free time as I am still in school and usually put whatever I learned into  
practice through xcl. This project is likely to change based on some design decisions because again this  
a learning avenue for me.
  

At the moment it is not complete so there are no documentation available publicly on how to use xcl.

# Steps outlined
* Tokenising / Lexing - this stage of the program is when it goes through a given source file line by line  
to create tokens of each "word". These tokens are stored in a Doubly Linked List that holds information  
about each token. Information such as Token Type, it's value and the line number it was generated from
are stored. (DONE)  
  
* Parsing - At this stage the entire source file has been successfully tokenised and is ready for parsing.  
This is where the program begins matching tokens against the next in order to generate an Abstract Syntax
Tree (AST). xcl being a simple language I didn't find the need to build one so I'll parse and write the logic
by hand while keeping track of what's where. My thoughts on building an AST might change in the future though
depending on how complex this gets.

* Error Handling - At the moment the code base is written in an optimistic fashion meaning I haven't done  
any error checking such as syntax errors, logical errors and such. My idea to implement such a featurs is by  
using another Linked List / Stack that keeps track of every error encountered and once we move to the Parsing  
stage, I check if the List contains any nodes. If it does, we'll output each error and their respective line.  
