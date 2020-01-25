//#include <mpi.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//const int MSG_SEND_STRING = 33;
//
//int main(int argc, char** argv) {
//
//	// Initializes the MPI environment
//	MPI_Init(NULL, NULL);
//
//	// Returns the size of a communicator. Our case: amount of processes that were requested for the job.
//	int world_size;
//	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//
//	// Returns the rank of a process in a communicator.
//	int world_rank;
//	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//
//	int message_len, source;
//	MPI_Status status;
//	if (world_rank != 0) {
//
//		// Prepares data to send
//		char str[50];
//		sprintf_s(str, "Hello from %d process ", world_rank);
//		for (int i = 0; i < world_rank; i++) {
//			strcat_s(str, "!");
//		}
//
//		// Sends data to the 0 process. 
//		// MPI_Send(
//		//  void* data,
//		//	int count,
//		//	MPI_Datatype datatype,
//		//	int destination,
//		//	int tag,
//		//	MPI_Comm communicator)
//		MPI_Send(str, (int)strlen(str) + 1, MPI_CHAR,
//			0, MSG_SEND_STRING, MPI_COMM_WORLD);
//	}
//	else {
//		printf("===== START MAIN PROCESS =====\n\n");
//		printf("MAIN. World rank: %d\n", world_rank);
//		printf("MAIN. World size: %d\n", world_size);
//
//		// Prepares result 2D char array. All processes except 0.
//		char** result = (char**)malloc(sizeof(char*) * (world_size - 1));
//
//		// Listens for the first incoming message and gets info from it 
//		for (int i = 1; i < world_size; i++) {
//			// MPI_Probe(
//			//	int source,
//			//	int tag,
//			//	MPI_Comm comm,
//			//	MPI_Status* status)
//			MPI_Probe(MPI_ANY_SOURCE, MSG_SEND_STRING, MPI_COMM_WORLD, &status);
//
//			//  MPI_Get_count(
//			//	MPI_Status * status,
//			//	MPI_Datatype datatype,
//			//	int* count)
//			MPI_Get_count(&status, MPI_CHAR, &message_len);
//			source = status.MPI_SOURCE;
//
//			// allocating memory for message 
//			result[source - 1] = (char*)malloc(sizeof(char) * message_len);
//
//			// MPI_Recv(
//			//  void* data,
//			//	int count,
//			//	MPI_Datatype datatype,
//			//	int source,
//			//	int tag,
//			//	MPI_Comm communicator,
//			//	MPI_Status* status)
//
//			//	MPI_Send sends the exact count of elements, and MPI_Recv will receive at most the count of elements
//			MPI_Recv(result[source - 1], message_len, MPI_CHAR, source,
//				MSG_SEND_STRING, MPI_COMM_WORLD, &status);
//			printf("Receive string from %d process: %d symbols\n", source, message_len);
//		}
//
//		// Show results
//		printf("\n===== PRINTING RESULTS =====\n\n");
//		for (int i = 0; i < world_size - 1; i++) {
//			printf("Process %d: %s\n", (i + 1), result[i]);
//		}
//
//		// Free memory
//		free(result);
//
//		printf("\n=====      ENDING      =====\n");
//	}
//
//	// Cleans up the MPI environment
//	MPI_Finalize();
//	return 0;
//}