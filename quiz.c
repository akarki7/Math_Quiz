#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>

char* readall (int fd);//reads data from the file descriptor fd until the EOF and returns all data in dynamically allocated string
char* fetch(); //creates a pipe and subsequently runs a child process to execute curl in order to fetch a question as JSON document
long get_answer();
char* get_question();
unsigned play(unsigned n, unsigned score, char *text, long answer); //unsigned means unsigned int

int main()
{
    printf("Answer questions with numbers in the range [1..100].\n\
You score points for each correctly answered question.\n\
If you need multiple attempts to answer a question, the\n\
points you score for a correct answer go down.\n");
    //do all the coding from here
    printf("\n");
    
    unsigned q_count=1;
    unsigned user_score=0;
    while(1)
    {
        char *redirect=fetch();
        long ans=get_answer(redirect);
        char* text=get_question(redirect);

        user_score=play(q_count,user_score,text,ans);
        printf("Your total score is %d/%d\n",user_score,8*q_count);
        q_count++;
        free(redirect);//freeing allocated memory of the functions
        free(text);
    }

    return 0;
}

char* readall (int fd)
{
    //use read() to read data from file descriptor
    //if read() returns 0 then it is EOF
    char buf;
    char* buf2;
    buf2 = (char*)malloc(1000 * sizeof(char));
    
    int val;
    int k=0;

    while(val=read(fd,&buf,1)>0)
    {
        buf2[k]=buf;
        k++;
    }
    buf2[k]='\0';//string terminator
    return buf2;
}

char* fetch()//creates a pipe;runs a child process to execute curl to fetch question;
{
    pid_t pid;
    char * buf2;

    int pfd[2];//two file descriptors for pipe (read and write)
    
    if (pipe(pfd) == -1) //pipe function creates a pipe
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    

    pid=fork();

    if (pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) //child process
    {
        //executes curl here
        //in write end so need to close the read end
        if (dup2(pfd[1],STDOUT_FILENO)==-1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        (void) close(pfd[0]);//closing the read end
        (void) close(pfd[1]);//closing the other end
        //use execlp here
        execlp("curl","curl","-s","http://numbersapi.com/random/math?min=1&max=100&fragment&json",NULL);
        //-s supresses output while redirecting s=silence
    }
    else //parent process
    {
        //reads data so need to close write end
        (void) close(pfd[1]); //closing the write first
        buf2=readall(pfd[0]);
        (void) close(pfd[0]);//closing other end first
    }
    return buf2;
}

long get_answer(char* str)
{
    char *ptr = str;
	long value;

	ptr = strstr(ptr, "\"number\"");
	//strstr(str1,str2) search for str2 in str1 and return pointer pointing to the first character of str2 found in str1

    ptr = strchr(ptr, ':');
	//strchr(string,character) is used to find occurence of a character in string

    ptr++;//move one step ahead of ':'
		
    value = strtol(ptr, &ptr, 10);

    return value;
}

char* get_question(char*str)
{
    char *ptr = str;
	char* value;
    value=(char*) malloc(1000*sizeof(char));
    ptr = strstr(ptr, "\"text\"");
	//strstr(str1,str2) search for str2 in str1 and return pointer pointing to the first character of str2 found in str1

    ptr = strchr(ptr, ':');
	//strchr(string,character) is used to find occurence of a character in string

    ptr++;//move one step ahead of ':'
	ptr++;//move ahead of ' '
	ptr++;//move ahead of '"'

    int k=0;
    
	while(*ptr != '"')
	{
		value[k]=*ptr;
		ptr++;
		k++;
	}
	value[k]='\0';
    
    return value;
}

unsigned play(unsigned n, unsigned score, char *text, long answer)
{
    long user_answer;

    printf("\nQ%d: What is %s?\n",n,text);
    int point = 8;
    while(point!=0)
    {
        printf("\n%d pt> ",point);
        if(scanf("%lupt> ",&user_answer)==EOF) //stop program if EOF is reached
        {
            printf("\nYour total score is %d/%d\n",score,8*(n-1));
            exit(EXIT_SUCCESS);
        }
        if (user_answer>100 || user_answer<1)
        {
            printf("Enter a value between 1 and 100!!\n");
            continue;//skip this loop as the input is invalid
        }
        if(user_answer==answer)
        {
            score=score+point;
            printf("Congratulation, your answer %lu is correct.\n",user_answer);
            break;
        }
        else
        {
            point=point/2;
            if(user_answer<answer)
            {
                if(point==0)
                {
                    printf("Too small, the correct answer was %lu.\n",answer);
                }
                else
                {
                    printf("Too small, try again.");
                }
            }
            else
            {
                if(point==0)
                {
                    printf("Too large, the correct answer was %lu.\n",answer);
                }
                else
                {
                    printf("Too large, try again.");
                }
            }
        }    
    }
    return score;
}
