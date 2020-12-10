/*Sourav Das, B00785668*/
#include <stdio.h>//including the header files
#include <stdlib.h>//including standard library
#include <unistd.h>//for miscellaneous
#include <string.h>//for strtok

//prototyping our methods for fcfs algorithm
void findStartingTime(int processes[], int n, int bt[],int st[]);
void findTurnAroundTime( int processes[], int n, int bt[], int st[], int tat[]);
void fcfs(FILE *fp1,int processes[], int n, int bt[]);
void findStartingTime2(int processes[], int n, int bt[],int st1[]);
void findTurnAroundTime2( int processes[], int n, int bt[], int st1[], int tat1[]);
void nsjf(FILE *fp1,int processID[],int processes[], int n, int bt[]);
void insertionSort(int arr[], int n);
void findStartEnd(int processID[], int n, int bt[], int wt2[], int quantum);
void roundRobin(FILE *fp1,int processID[], int n, int bt[], int quantum);

//some global variables
int from[200];
int task[200];
int to[200];
int count=0;


int main(int argc, const char* argv[])//storing the arguments and the number of arguments
{
    char line[20];//could store upto 20 characters for each line
    char* array[3];//since there would be 3 elements on each line
    int processes[50];
    int bt[50];

    FILE *fp, *fp1;//first one to read from, second one to write to
    fp = fopen("TaskSpec.txt" , "r");//opening the given file and reading it

    int n=0;
    while(fgets(line,sizeof(line),fp)!=NULL){//till there is any line left, the loop continues
    char delimeter[] = ",";//, is the delimiter in our case
    char *p = strtok(line, delimeter);
	int m=0;//index of the array


	while(p != NULL)//tokenize whole input by delimiter and store them in the array
	{
		array[m]= (p);//storing the token in our array
		p = strtok(NULL, delimeter);//collects the end of input tokens

		if(p!=NULL)//only if the token is not null
		m++;//incrementing index
	}

        for(int k=0;k<=m;k++){

        if(k==1){
            processes[n]=atoi(array[k]);//creating an array for the arrival times

        }
        if(k==2){
            bt[n]=atoi(array[k]);//creating an array for burst times

        }

        }
        printf("\n");//next line
        n++;//incrementing the count of task or process
    }
    int processID[n];//this array puts an ID to the processes according to their arrival time
    for(int i=0;i<n;i++){
        processID[i]=i+1;
    }

    //it is opening text file to write on
    fp1 = fopen("Output.txt" , "w");

    fcfs(fp1,processes, n,  bt);//calling the fcfs method to implement FCFS algorithm

    int quantum = 4;//declaring the quantum
    roundRobin(fp1,processID, n, bt, quantum);//calling the round robin method to implement round robin algorithm

    nsjf(fp1,processID,processes, n, bt);//calling the nsjf method to implement NSJF algorithm


    fclose(fp1);//closing the writing file

    return 0;//return 0 to main function
}


//to count the starting time
void findStartingTime(int processes[], int n, int bt[],int st[])
{

    st[0] = 0;//starting time for the first process is zero

    for(int i=1;i<n;i++)
    {
        st[i]=0;
        for(int j=0;j<i;j++)
            st[i]+=bt[j];//by adding to the bursting time of the last process we find the starting time for next process
    }
}



void findTurnAroundTime( int processes[], int n, int bt[], int st[], int tat[])
{

    for (int i = 0; i < n ; i++)//for n times which is the number of processes
    {
        tat[i] = st[i]+bt[i];//adding the starting time and burst time, we get the turn around time
    }

}



//fcfs method
void fcfs(FILE *fp1,int processes[], int n, int bt[])
{
    int st[n], tat[n], wt[n];
    int total_wt = 0, total_tat = 0;


   findStartingTime(processes, n, bt, st);//calling function to retrieve the starting times

   findTurnAroundTime(processes, n, bt, st, tat);//calling functions to retrieve the turn around times


    //printf("\nFCFS:\n"); //from here the display part
    fprintf(fp1,"\nFCFS:\n"); //from here the display part


    for (int i = 0; i < n; i++)
    {
        //printf( "T%d\t\t\t%d\t\t\t%d\n" ,(i+1),st[i],tat[i]); //printing the starting and ending time
        fprintf(fp1,"T%d\t\t%d\t\t%d\n" ,(i+1),st[i],tat[i]); //printing the starting and ending time
    }


    for(int i=0; i<n;i++){
            wt[i]=st[i]-processes[i];//waiting time is calculated
            total_wt = total_wt+wt[i];//total weighting time is calculated
        //printf("Waiting time for T%d: %d\n",i+1,wt[i]);//printing waiting time of each process
        fprintf(fp1,"Waiting time T%d: %d\n",i+1,wt[i]);//printing waiting time of each process
    }
    //printf("Average waiting time: %.2f\n\n\n",(float)total_wt/(float)n);//printing the average waiting time
    fprintf(fp1,"Average waiting time: %.2f\n\n\n",(float)total_wt/(float)n);//printing the average waiting time
}

//this method is just an insertion sort
void insertionSort(int array[], int n)
{
    int i, k, j;
    for (i = 1; i < n; i++) {
        k = array[i];
        j = i - 1;

        while (array[j] > k && j >= 0) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = k;
    }
}

//method for the npsjs method to find the starting times of the processes
void findStartingTime2(int processes[], int n, int bt[],int st1[])
{
int sort[n-1];//creating a duplicate array for the burst times except the first one
for(int i=1;i<n;i++){
    sort[i-1]=bt[i];
}

insertionSort(sort, n-1);//sorting the duplicate array

    st1[0] = 0;//starting time for the first process is zero
    st1[1]=bt[0];//the second starting time is the burst time of the first process
    for(int i=2;i<n;i++)//for the rest of the processes
    {
        st1[i]=0;
        for(int j=0;j<i-1;j++)
        st1[i]=st1[i-1]+sort[j];//by adding to the bursting time of the last process we find the starting time for next process
    }
}


//method for the npsjs method to find the end time of the processes
void findTurnAroundTime2( int processes[], int n, int bt[], int st1[], int tat1[]){
int sorted[n-1];//creating duplicate array for the burst times
for(int i=1;i<n;i++){
    sorted[i-1]=bt[i];
}

insertionSort(sorted, n-1);//sorting the duplicate array

tat1[0] = st1[0]+bt[0];//creating an array to hold the turn around times
    for (int i = 1; i < n ; i++)
    {
            tat1[i]=st1[i]+sorted[i-1];
    }

}


//npsjs method
void nsjf(FILE *fp1,int processID[],int processes[], int n, int bt[]){

    int st1[n], tat1[n], wt1[n];
    int total_wt1 = 0, total_tat1 = 0;



   findStartingTime2(processes, n, bt, st1);//calling function to retrieve the starting times


   findTurnAroundTime2(processes, n, bt, st1, tat1);//calling functions to retrieve the turn around times


   int temp,pos,temp1;
   int procpy[n],procpy2[n];

   for(int i=0;i<n;i++){//creating a duplicate array for the process IDs
    procpy[i]=processID[i];
   }

   for(int i=0;i<n;i++){//creating a duplicate array for the arrival times
    procpy2[i]=processes[i];
   }

   for(int i=1;i<n;i++)//just sorting the indices of the duplicate arrays based on the sequence in burst time array
    {
        pos=i;
        for(int j=i+1;j<n;j++)
        {
            if(bt[j]<bt[pos])
                pos=j;
        }

        temp=procpy[i];
        procpy[i]=procpy[pos];
        procpy[pos]=temp;

        temp1=procpy2[i];
        procpy2[i]=procpy2[pos];
        procpy2[pos]=temp1;
    }


    //printf("\nNPSJF:\n"); //from here the display part
    fprintf(fp1,"\nNPSJF:\n"); //from here the display part

    for (int i = 0; i < n; i++)
    {
        //printf( "T%d\t\t\t%d\t\t\t%d\n" ,procpy[i],st1[i],tat1[i]); //printing the starting and ending time
        fprintf(fp1,"T%d\t\t%d\t\t%d\n" ,procpy[i],st1[i],tat1[i]); //printing the starting and ending time
    }


    for(int i=0; i<n;i++){//for each of the processes
            wt1[i]=st1[i]-procpy2[i];//waiting time is calculated
            total_wt1 = total_wt1+wt1[i];//total weighting time is calculated
        //printf("Waiting time for T%d: %d\n",procpy[i],wt1[i]);//printing waiting time of each process
        fprintf(fp1,"Waiting time T%d: %d\n",procpy[i],wt1[i]);//printing waiting time of each process
    }

    //printf("Average waiting time: %.2f\n",(float)total_wt1/(float)n);//printing the average waiting time
    fprintf(fp1,"Average waiting time: %.2f\n\n",(float)total_wt1/(float)n);//printing the average waiting time
}



void findStartEnd(int processID[], int n, int bt[], int wt2[], int quantum){//this method finds the start and end times for rr
    int temp_bt[n],t;
    for (int i = 0 ; i < n ; i++)//creating duplicate array for the burst times
    temp_bt[i] =  bt[i];
    t = 0;

    // until all of the processes are not finished
    while (1){
    int mark = 1;
        // Traverse all the processes
        for (int i = 0 ; i < n; i++){
            if (temp_bt[i] > 0){//if the burst time is greater than zero
                mark = 0; // There is a pending process

                if (temp_bt[i] > quantum){//if the burst time is greater than the quantum

                    from[count]=t;//tracking the starting time
                    t =t+quantum;//tracking the total time
                    task[count]=processID[i];//tracking the process ID
                    to[count]=t;//tracking the end of the execution
                    count++;//incremeting the execution number

                    temp_bt[i] = temp_bt[i]-quantum;//reducing the quantum from the burst time
                }


                else
                {
                    //work similar like the if statement above
                    from[count]=t;
                    t = t + temp_bt[i];
                    task[count]=processID[i];
                    to[count]=t;
                    count++;
                    temp_bt[i] = 0;//the burst time is zero as the process is finished

                    wt2[i] = t - bt[i];//calculating the weighting time for this process
                }
            }

        }
        if (mark == 1)
        break;
    }
}



// roundRobin function
void roundRobin(FILE *fp1,int processID[], int n, int bt[], int quantum){
    int wt2[n];//creating an array to hold the waiting times for each process

    fprintf(fp1,"RR:\n");//prints headline
    // to find start and end of all processes
    findStartEnd(processID, n, bt, wt2, quantum);

    for (int i=0; i<count; i++)
    {
      fprintf(fp1,"T%d\t\t%d\t\t%d\n", task[i], from[i], to[i]);//printing the process id, start and end times
    }

     for(int i=0; i<n; i++){
     fprintf(fp1,"Waiting time T%d: %d\n",processID[i],wt2[i]);//printing the waiting times for each process
     }

     int total=0;
     for(int i=0;i<n;i++){
        total=total+wt2[i];//calculating the total weighting time
     }
    fprintf(fp1,"Average waiting time= %.2f\n\n", (float)total / (float)n);//printing the average waiting time
}
