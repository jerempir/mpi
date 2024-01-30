#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;


vector<int> vec(int size, int seed){
    std::minstd_rand simple_rand;
    simple_rand.seed(seed);
    vector<int> v(size);
    for(int i = 1;i<=size;i++){
        int a = abs(int(simple_rand() % 100000 + 1));
        v[i-1] = a;
    }
    return v;
}

int main(int argc, char **argv)
{ 
    long long  w = 100000000;
    MPI_Init(&argc, &argv);
    float t1 = MPI_Wtime();
    for(int n = w;n<=w;n*=10){
    int rank = -1;
    int cp = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cp);
    if(rank == 0){
        // print_vec(v1);
        // cout<<endl;
        // print_vec(v2);
        // cout<<endl;
        auto v1 = vec(n,1);
        auto v2 = vec(n,17);
        for(int i = 1;i<cp;i++){
            MPI_Send(&v1[0],n,MPI_INT,i,rank,MPI_COMM_WORLD);
            MPI_Send(&v2[0],n,MPI_INT,i,rank,MPI_COMM_WORLD);
        }
        vector<int> res(cp,0);
        MPI_Status Status;
        for(int i = 0; (floor(n/(cp-1))+1)*i < n; i++){
            MPI_Recv(&res[i], 1, MPI_INT, i+1, MPI_ANY_TAG, MPI_COMM_WORLD, &Status); 
        }
        int q = 0;
        for(int i = 0;i<cp;i++){
            q += res[i];
        }
        cout<<MPI_Wtime() - t1<<","<<endl;
    }
    else{
        vector<int> v1(n);
        vector<int> v2(n);
        MPI_Status Status;
        MPI_Recv(&v1[0], n, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status); 
        MPI_Recv(&v2[0], n, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status); 
        int s = 0;
        if((floor(n/(cp-1))+1)*(rank-1) < n){
            for(int i = (floor(n/(cp-1))+1)*(rank-1); (i<(floor(n/(cp-1))+1)*(rank)) and (i < n); i++)
                s += v1[i] * v2[i];
            MPI_Send(&s,1,MPI_INT,0,rank,MPI_COMM_WORLD);
        }
    }
    }
    MPI_Finalize();
}