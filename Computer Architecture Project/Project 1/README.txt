Name: Jonathan Kim
NYU Email: jmk9720@nyu.edu
State of the work: Complete
Resources: None
Justification of the Design:
The opcode was split into two major types: three register opcode and two register opcode. This is because the E20 manual
also split the opcodes into two major types. Also, three register opcode instructions have an opcode of 000 while two 
register opcode instructions have opcodes greater than 3 (with an exception of addi). The addi operation was done in a 
separate else if case because it did not agree with the case of opcode greater than 3. Anything that had an opcode of 2 or 3
was a jump operation that had a separate else if case. A function to convert 2's complement number from unsigned numbers 
made things easier to code since the immediates in the two register operations are sign-extended, meaning the immediates were
signed numbers. There are no noticeable strengths or weaknesses in my code.