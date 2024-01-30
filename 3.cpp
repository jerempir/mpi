#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv)
{   
    MPI_Init(&argc, &argv);
    long long  w = 10000000;
    for(int n = 10000;n<=w;n*=10){
    float t1 = MPI_Wtime();
    long long q = 1000;
    int rank = -1;
    int cp = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cp);
    if(rank == 0){
        vector<int> res(n);
        MPI_Status Status;
        for(int i = 0; i<q; i++){
            MPI_Recv(&res[0], n, MPI_INT, 17, MPI_ANY_TAG, MPI_COMM_WORLD, &Status); 
            //for(int i = 0;i<n;i++)
            //    cout<<res[i]<<" ";
            //cout<<endl;
        }
        cout<<MPI_Wtime() - t1<<","<< endl;
    }
    if(rank == 1){
        vector<int> s(n);
        for(int i = 0;i<n;i++)
            s[i] = 1;
        for(int i = 0;i<q;i++)
            MPI_Send(&s[0],n,MPI_INT,0,rank,MPI_COMM_WORLD);
    }

    }
    MPI_Finalize();
}