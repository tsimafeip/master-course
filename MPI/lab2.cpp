//#include <mpi.h>
//#include <iostream>
//#include <string>
//#include <cmath>
//
//const int MSG_SEND_DOUBLE = 33;
//
//double f(double x) {
//	return cos(x);
//}
//
//void send_idx(int idx, int dest) {
//	MPI_Send(&idx, 1, MPI_INT, dest, MSG_SEND_DOUBLE, MPI_COMM_WORLD);
//}
//
//int main(int argc, char** argv) {
//	int world_rank, world_size, master = 0, n = std::stoi(argv[1]), r = std::stoi(argv[2]), FINALIZE = -1;
//	int a = 0, b = 1;
//	double long_step = (b - a) * 1.0 / n;
//	double short_step = long_step / r;
//
//	MPI_Init(NULL, NULL);
//
//	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//
//	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//
//	if (world_rank != master) {
//		int i = 0;
//		double partial_sum = 0.0;
//		while (true) {
//			// MPI_Send(
//			//  void* data,
//			//	int count,
//			//	MPI_Datatype datatype,
//			//	int destination,
//			//	int tag,
//			//	MPI_Comm communicator)
//			MPI_Send(&partial_sum, 1, MPI_DOUBLE, master, MSG_SEND_DOUBLE, MPI_COMM_WORLD);
//			// MPI_Recv(
//			//  void* data,
//			//	int count,
//			//	MPI_Datatype datatype,
//			//	int source,
//			//	int tag,
//			//	MPI_Comm communicator,
//			//	MPI_Status* status)
//			MPI_Recv(&i, 1, MPI_INT, master, MSG_SEND_DOUBLE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//			if (i == FINALIZE) {
//				break;
//			}
//
//			double current_segment_start = a + i * long_step;
//			partial_sum = 0;
//			for (int r_i = 0; r_i < r; r_i++) {
//				partial_sum += f(current_segment_start + r_i*short_step);
//			}
//		}
//	}
//	else {
//		int processed_segments = 0, finalized_processes = 0;
//		double final_sum = f(a), partial_sum;
//		MPI_Status slave_status;
//		// World size - amount of the working processes. We should extract master process and do not forget that process count starts from 1.
//		while (finalized_processes < world_size-1) {
//			MPI_Recv(&partial_sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MSG_SEND_DOUBLE, MPI_COMM_WORLD, &slave_status);
//			final_sum += partial_sum;
//			if (processed_segments >= n) {
//				send_idx(FINALIZE, slave_status.MPI_SOURCE);
//				finalized_processes++;
//				std::cout << "Finalized process " << finalized_processes << std::endl;
//			}
//			else {
//				send_idx(processed_segments++, slave_status.MPI_SOURCE);
//			}
//		}
//
//		std::cout << "Sum: " << final_sum * short_step << std::endl;
//	}
//
//	MPI_Finalize();
//
//	return 0;
//}