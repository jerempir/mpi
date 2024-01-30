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

int main(int argc, char **argv){  
    int w = 100000000;
    MPI_Init(&argc, &argv);
    for(int n = 10000;n<=w;n*=10){
        int rank = -1;
        int cp = -1;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &cp);
        if(rank == 0){
            auto v1 = vec(n,1);
            float t1 = MPI_Wtime();
            for(int i = 1;i<cp;i++){
                MPI_Send(&v1[0],n,MPI_INT,i,rank,MPI_COMM_WORLD);
            }
            vector<int> res(cp);
            for(int i = 0;i<cp;i++)
                res[i] = 1000000;
            MPI_Status Status;
            for(int i = 0; (floor(n/(cp-1))+1)*i < n; i++){
                MPI_Recv(&res[i], 1, MPI_INT, i+1, MPI_ANY_TAG, MPI_COMM_WORLD, &Status); 
            }
            int q = 1000000;
            for(int i = 0;i<cp;i++){
                q = min(q,res[i]);
            }
            
            //cout<<"ans = "<<q<<endl;
            cout<<MPI_Wtime() - t1<<","<<endl;
        }
        else{
            vector<int> v1(n,0);
            int s = 1000000;
            MPI_Status Status;
            MPI_Recv(&v1[0], n, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status); 
            if((floor(n/(cp-1))+1)*(rank-1) < n){
                for(int i = (floor(n/(cp-1))+1)*(rank-1); (i<(floor(n/(cp-1))+1)*(rank)) and (i < n); i++)
                    s = min(s, v1[i]);
                MPI_Send(&s,1,MPI_INT,0,rank,MPI_COMM_WORLD);
            }
        }
    }
    MPI_Finalize();
}