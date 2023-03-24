#include <mpi.h>
#include "matrix.hpp"

using namespace std;

// tags
#define ROW_START_TAG 0
#define ROW_END_TAG 1
#define ROWS_TAG 2
#define ANS_ROWS_TAG 3

int main (int argc, char* argv[]) {

  // Data initialization
  fstream f("./input.txt");
  Data<int> d;
  d.ReadFromFile(f);
  int numOfRows = d.rows();
  int numOfCols = d.cols();
  vector<int> ans(numOfRows, 0);

  int ProcNum, ProcRank;
  int rank;
  MPI_Status status;
  int rowStart, rowEnd;
  int granularity;
  double start_time, end_time;
  double localTimeSaver;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcRank == 0) {
    start_time = MPI_Wtime();
    for (int i = 1; i < ProcNum; i++) {     
      granularity = (numOfRows / (ProcNum - 1)); 
      rowStart = (i - 1) * granularity;

      if (((i + 1) == ProcNum) && ((numOfRows % (ProcNum - 1)) != 0)) {
        rowEnd = numOfRows; //last slave gets all the remaining rows
      } else {
        rowEnd = rowStart + granularity; //rows are equally divisable among slaves
      }

      // MPI_Send(buf, count, datatype, dest, tag, comm,ierr)
      MPI_Send(&rowStart, 1, MPI_INT, i, ROW_END_TAG, MPI_COMM_WORLD);
      MPI_Send(&rowEnd, 1, MPI_INT, i, ROW_START_TAG, MPI_COMM_WORLD);
    }
  }

  if (ProcRank > 0) {
    MPI_Recv(&rowStart, 1, MPI_INT, 0, ROW_END_TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(&rowEnd, 1, MPI_INT, 0, ROW_START_TAG, MPI_COMM_WORLD, &status);

    for (int i = rowStart; i < rowEnd; i++) {
      for (int j = 0; j < d.cols(); j++) {
          ans[i] += d(i, j) * d.get_vec()[j];
      }
    }
  }

  MPI_Finalize();
  return 0;
}
