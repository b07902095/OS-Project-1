#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>//gettimeofday
#include <unistd.h>//fork
void swap(int * a, int * b){
	int c = *a;
	*a = *b;
	*b = c;
}
void sort(int value[], int index[], int N, int ascend){
	if(ascend){
		for(size_t i = 0; i < N; ++i){
			for(size_t j = i + 1; j < N; ++j){
				if(value[i] > value[j]){
					swap(&value[i], &value[j]);
					swap(&index[i], &index[j]);
				}
			}
		}
	}
	else{
		for(size_t i = 0; i < N; ++i){
			for(size_t j = i + 1; j < N; ++j){
				if(value[i] < value[j]){
					swap(&value[i], &value[j]);
					swap(&index[i], &index[j]);
				}
			}
		}
	}
}
void inverse_permutation(const int a[], int b[], int N){
	int count = 0, cursor = 0, probe = 0;
	for(size_t i = 0; i < N; ++i)
		b[i] = -1;
	
	while(count < N){
		if(b[a[probe]] == -1){
			b[a[probe]] = probe;
			probe = a[probe];
			++count;
		}
		else{
			for(; cursor < N; ++cursor)
				if(b[cursor] == -1){
					probe = cursor;
					break;
				}
		}
	}
}
void PSJF(int *R, int *R_index, int *T, int *T_inverse, int N) 
{
	
	
	//int ready = 0;//# of process arrived and not finished
	int count = 0;//# of process arrived
	
	int startTime = 0;
	//struct timeval t1, t2;
	int remain = N;//# of process not finished;
	int former = -2;
	while (remain > 0) 
	{
		while(count < N && R[count] <= startTime)
		{
			count++;
			//ready++;
		}
		
		if(N - count == remain)//no previous process remain
		{
			startTime = R[count];
		}
		while(count < N && R[count] <= startTime) 
		{
			count++;
			//ready++;
		}
		int shortest = -1;//find the shortest burst time process index
		for (size_t j = 0; j < count; j++) 
		{
			if (T[T_inverse[R_index[j]]] != 0)//process unfinished 
			{
				if (shortest == -1) 
				{
					shortest = R_index[j];
				}
				else if(T[T_inverse[R_index[j]]] < T[T_inverse[shortest]])
				{
					shortest = R_index[j];
				}
			}
		}
		if(shortest != former)
		{
			printf("P%d\n", shortest + 1);
		}
		former = shortest;
		int runtime = T[T_inverse[shortest]];//max time the process can run 
		if(count < N && R[count] < startTime + runtime)//next process arrived before finished
		{
			runtime = R[count] - startTime;
		}
		startTime += runtime;
		if(runtime == T[T_inverse[shortest]])//finished
		{
			remain --;
			T[T_inverse[shortest]] = 0;
		}
		else
		{
			T[T_inverse[shortest]] -= runtime;
		}
		
	}
}
void SJF(int *R, int *R_index, int *T, int *T_inverse, int N) 
{
	
	
	int ready = 0;//# of process arrived and not finished
	int count = 0;//# of process arrived
	
	int startTime = 0;
	//struct timeval t1, t2;

	for (size_t i = 0; i < N; i++) 
	{
		while(count < N && R[count] <= startTime)
		{
			count++;
			ready++;
		}
		if (ready == 0)//continue 
		{
			startTime = R[count];
		}
		while(count < N && R[count] <= startTime) 
		{
			count++;
			ready++;
		}
		int shortest = -1;//find the shortest burst time process index
		for (size_t j = 0; j < count; j++) 
		{
			if (T[T_inverse[R_index[j]]] != 0)//process unfinished 
			{
				if (shortest == -1) 
				{
					shortest = R_index[j];
				}
				else if(T[T_inverse[R_index[j]]] < T[T_inverse[shortest]])
				{
					shortest = R_index[j];
				}
			}
		}
		printf("P%d\n", shortest + 1);
		startTime += T[T_inverse[shortest]];
		T[T_inverse[shortest]] = 0;//finished
		ready--;
	}
}
int next(int t, int N)
{
	int a = t;
	if(a == N - 1)
	{
		a = 0;
	}
	else
	{
		a = a + 1;
	}
	return a;
}
void RR(int *R, int *R_index, int *T, int *T_inverse, int N) 
{
	
	
	int count = 0;//# of process arrived
	
	int startTime = 0;
	int remain = N;//# of process not finished;
	int ready[N];
	int head = 0;
	int tail = 0;

	int quantum = 20;	
	
	while (remain > 0) 
	{
		while(count < N && R[count] <= startTime)
		{
			ready[tail] = R_index[count++];
			//count++;
			tail = next(tail, N);
		}

		if(N - count == remain)//no previous process remain
		{
			startTime = R[count];
		}
		while(count < N && R[count] <= startTime)
		{
			ready[tail] = R_index[count++];
			//count++;
			tail = next(tail, N);
		}
		if(T[T_inverse[ready[head]]] <= quantum)
		{
			printf("P%d\n", ready[head] + 1);
			startTime += T[T_inverse[ready[head]]];
			while(count < N && R[count] <= startTime)
			{
				ready[tail] = R_index[count++];
				//count++;
				tail = next(tail, N);
			}
			T[T_inverse[ready[head]]] = 0;
			head = next(head, N);
			remain --;
		}
		else
		{
			printf("P%d\n", ready[head] + 1);
			startTime += quantum;
			while(count < N && R[count] <= startTime)
			{
				ready[tail] = R_index[count++];
				//count++;
				tail = next(tail, N);
			}
			T[T_inverse[ready[head]]] -= quantum;
			ready[tail] = ready[head];
			head = next(head, N);
			tail = next(tail, N);
		}

		
	}
}
int main()
{
	char S[4];
	int N;
	
	scanf("%s", S);
	scanf("%d", &N);
	
	char P[N][32];
	int R[N], T[N];
	int R_index[N], T_index[N], R_inverse[N], T_inverse[N];
	for(size_t i = 0; i < N; ++i)
		R_index[i] = T_index[i] = i;
	
	for(size_t i = 0; i < N; ++i){
		scanf("%s", P[i]);
		scanf("%d", &R[i]);
		scanf("%d", &T[i]);
	}
	sort(R, R_index, N, 1);
	sort(T, T_index, N, 1);
	inverse_permutation(R_index, R_inverse, N);
	inverse_permutation(T_index, T_inverse, N);
	if(strcmp(S, "SJF") == 0)
	{
		SJF(R, R_index, T, T_inverse, N);
	}
	else if(strcmp(S, "PSJF") == 0)
	{
		PSJF(R, R_index, T, T_inverse, N);
	}
	else if(strcmp(S, "RR") == 0)
	{
		RR(R, R_index, T, T_inverse, N);
	}


	return 0;
}




