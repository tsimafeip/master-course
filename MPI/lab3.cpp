//#include <mpi.h>
//#include <random>
//#include <iostream>
//#include <time.h>
//#include <string>
//
//using namespace std;
//
//const int MSG_SEND_INT = 10;
//
//void print_lines(int** a, int lines, int columns) {
//	for (int i = 1; i < lines; ++i) {
//		for (int j = 0; j < columns; ++j) {
//			cout << a[i][j] << "\t";
//		}
//		cout << endl;
//	}
//}
//
//int main(int argc, char** argv) {
//	int world_size, world_rank, master = 0, N1 = stoi(argv[1]), N2= stoi(argv[2]), r = stoi(argv[3]);
//	MPI_Status status;
//	MPI_Request request1 = MPI_REQUEST_NULL, request2 = MPI_REQUEST_NULL;
//
//	MPI_Init(NULL, NULL);
//
//	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//
//	int average_lines_per_process = (N1 + (world_size - 1)) / world_size;
//	int lines_number_per_process = min(average_lines_per_process, N1 - world_rank * average_lines_per_process) + 1;
//
//	auto* memory = new int[lines_number_per_process * N2];
//	auto** a = new int* [lines_number_per_process];
//
//	for (int i = 0; i < lines_number_per_process; ++i) {
//		a[i] = &memory[i * N2];
//	}
//
//	srand(time(NULL));
//
//	for (int jb = 0; jb < N2; jb += r) {
//		int block_size = min(r, N2 - jb);
//
//		// Filling first line by blocks with random data
//		if (world_rank == master) {
//			for (int j = jb; j < jb + block_size; j++) {
//				a[0][j] = (rand() % 100);
//			}
//		}
//		else {
//			if (jb == 0) {
//				// int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
//				// int tag, MPI_Comm comm, MPI_Request* request)
//				MPI_Irecv(a[0], block_size, MPI_INT, world_rank - 1, MSG_SEND_INT, MPI_COMM_WORLD, &request1);
//			}
//			MPI_Wait(&request1, &status);
//
//			if (jb + r < N2) {
//				MPI_Irecv(a[0] + jb + r, block_size, MPI_INT, world_rank - 1, MSG_SEND_INT, MPI_COMM_WORLD, &request1);
//			}
//		}
//
//		for (int i = 1; i < lines_number_per_process; i++) {
//			for (int j = jb; j < jb + block_size; j++) {
//				a[i][j] = a[i - 1][j] + 1;
//			}
//		}
//
//		// Last process does not send any info
//		if (world_rank < world_size - 1) {
//			MPI_Wait(&request2, &status);
//			MPI_Isend(a[lines_number_per_process - 1] + jb, block_size, MPI_INT, world_rank + 1, MSG_SEND_INT, MPI_COMM_WORLD, &request2);
//		}
//	}
//	MPI_Wait(&request2, &status);
//
//	int print_code = -1;
//	if (world_rank != master) {
//		MPI_Recv(&print_code, 1, MPI_INT, world_rank - 1, MSG_SEND_INT, MPI_COMM_WORLD, &status);
//	}
//
//	printf("Results for %d process:\n", world_rank);
//	print_lines(a, lines_number_per_process, N2);
//	if (world_rank < world_size - 1) {
//		MPI_Send(&print_code, 1, MPI_INT, world_rank + 1, MSG_SEND_INT, MPI_COMM_WORLD);
//	}
//
//	MPI_Finalize();
//	return 0;
//}
