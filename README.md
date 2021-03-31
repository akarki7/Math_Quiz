# Math_Quiz
Simple and fun math quiz

## Steps to run the program
1) Open the terminal in the current directory of the file.
2) gcc -o quiz quiz.c
3) ./quiz

After this the program runs and you are asked to input your answers that is between 1 and 100. You are given 4 chances and in each chance the points you can earn decrease by 1/2 from before. For each question the highest you can earn is 8 points. If you want to end the game you need to press `Ctrl+ D`. After the game finishes you get your final score.

## About the code
1) For to fetch the questions I have used 'curl'. Once I made a curl request to the URL it returned my a JSON document with the question and its answers.
2) I have created a pipe which subsequently runs a child process to  execute curl in order to fetch a question as a JSON document. The result produced by the  child process executing curl is directed into the write end of the pipe. The parentprocess reads the data from the read end of the pipe 
3) Basically program implements a loop fetching a question, parsing it, and then playing oneround of the game. 
