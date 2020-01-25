#include<stdlib.h>
#include<ctime>
#include<math.h>
#include"mpi.h"
#include<iostream>

#pragma region GlobalVariables

#define MATRIX_SIZE 4

int first_matrix[MATRIX_SIZE][MATRIX_SIZE];
int second_matrix[MATRIX_SIZE][MATRIX_SIZE];
#pragma endregion

#pragma region GridStruct
typedef struct {
	int proc_count;
	int dim;
	int row;
	int col;
	int rank;
	MPI_Comm grid_comm;
	MPI_Comm row_comm;
	MPI_Comm col_comm;
} GridStructure;

void GridSetup(GridStructure* grid) {
	int dimensions[2];
	int wrap_around[2];
	int coordinates[2];
	int free_coords[2];

	MPI_Comm_size(MPI_COMM_WORLD, &(grid->proc_count));

	// setting grid dimension. 
	grid->dim = (int)sqrt((double)grid->proc_count);
	dimensions[0] = dimensions[1] = grid->dim;

	wrap_around[0] = 0;
	wrap_around[1] = 1;

	// Makes a new communicator to which topology information has been attached
	// int MPI_Cart_create(
	//	MPI_Comm comm_old,
	//	int ndims,
	//	int* dims, - integer array of size ndims specifying the number of processes in each dimension
	//	int* periods, - logical array of size ndims specifying whether the grid is periodic (true) or not (false) in each dimension
	//	int reorder, - process reordering
	//	MPI_Comm * comm_cart
	//);
	MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, 1, &(grid->grid_comm));
	MPI_Comm_rank(grid->grid_comm, &(grid->rank));
	// Determines process coords in cartesian topology given rank in group
	MPI_Cart_coords(grid->grid_comm, grid->rank, 2, coordinates);
	grid->row = coordinates[0];
	grid->col = coordinates[1];
	//printf("Grid row%d\n", grid->row);
	//printf("Grid col%d\n", grid->col);

	free_coords[0] = 0;
	free_coords[1] = 1;
	// Partitions a communicator into subgroups which form lower-dimensional cartesian subgrids
	MPI_Cart_sub(grid->grid_comm, free_coords, &(grid->row_comm));

	free_coords[0] = 1;
	free_coords[1] = 0;
	MPI_Cart_sub(grid->grid_comm, free_coords, &(grid->col_comm));
}
#pragma endregion

#pragma region Helpers
void UnpackMatrix(int* buff, int** a, int size) {
	int k = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			a[i][j] = buff[k];
			k++;
		}
}

void PackMatrix(int* buff, int** a, int size) {
	int k = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			buff[k] = a[i][j];
			k++;
		}
}

void PrintMatrix(int** matrix, int size) {
	for (int i = 0; i < size; i++) {
		std::cout << "|";
		for (int j = 0; j < size; j++) {
			int el = matrix[i][j];
			std::cout << el;
			std::cout << "|";
		}
		std::cout << std::endl;
	}
}

void PrintPackedMatrix(int* matrix, int size) {
	for (int i = 0; i < size; i++) {
		std::cout << "|";
		for (int j = 0; j < size; j++) {
			int el = matrix[i * size + j];
			std::cout << el;
			std::cout << "|";
		}
		std::cout << std::endl;
	}
}

void GenerateMatrices() {
	srand(time(nullptr));
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++) {
			first_matrix[i][j] = rand() % 100;
			second_matrix[i][j] = rand() % 100;
		}
}

void MultiplyLocal(int** a, int** b, int** c, int size) {
	int temp = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			temp = 0;
			for (int k = 0; k < size; k++)
				temp += (a[i][k] * b[k][j]);
			c[i][j] += temp;
		}
}
#pragma endregion

void FoxMultiply(int submat_dim, GridStructure* grid, int** a, int** b, int** c) {
	int** temp_a, * buff, k, root, b_src, b_dst;
	MPI_Status status;

	temp_a = new int* [submat_dim];
	for (int i = 0; i < submat_dim; ++i)
		temp_a[i] = new int[submat_dim];
	for (int i = 0; i < submat_dim; i++)
		for (int j = 0; j < submat_dim; j++)
			temp_a[i][j] = 0;

	buff = new int[submat_dim * submat_dim];
	for (int i = 0; i < submat_dim * submat_dim; i++)
		buff[i] = 0;

	b_src = (grid->row + 1) % grid->dim;
	b_dst = (grid->row + grid->dim - 1) % grid->dim;
	//printf("Src %d\n", src);
	//printf("Dst %d\n", dst);

	for (k = 0; k < grid->dim; k++) {
		root = (grid->row + k) % grid->dim;
		auto a_mul = root == grid->col ? a : temp_a;
		PackMatrix(buff, a_mul, submat_dim);

		// Broadcasts a message from the process with rank "root" to all other processes of the communicator
		//  int MPI_Bcast(
		//	void* buffer, - [in/out] starting address of buffer (choice)
		//	int count, [in] number of entries in buffer (integer)
		//	MPI_Datatype datatype,
		//	int root, - rank of broadcast root (integer)
		//	MPI_Comm comm
		//);

		// Although the root process and receiver processes do different jobs, 
		// they all call the same MPI_Bcast function. When the root process
		// calls MPI_Bcast, the data variable will be sent to all other processes.
		// When all of the receiver processes call MPI_Bcast, the data variable will be filled in with the data from the root process.
		MPI_Bcast(buff, submat_dim * submat_dim, MPI_INT, root, grid->row_comm);
		UnpackMatrix(buff, a_mul, submat_dim);
		MultiplyLocal(a_mul, b, c, submat_dim);

		PackMatrix(buff, b, submat_dim);
		// Sends and receives using a single buffer
		// int MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype,
		// int dest, int sendtag, int source, int recvtag,
		//	MPI_Comm comm, MPI_Status* status)
		MPI_Sendrecv_replace(buff, submat_dim * submat_dim, MPI_INT, b_dst, 0, b_src, 0, grid->col_comm, &status);
		UnpackMatrix(buff, b, submat_dim);
	}
}

int** TestSingleThread()
{
	int** test_result, ** test_a, ** test_b;
	test_a = new int* [MATRIX_SIZE];
	test_b = new int* [MATRIX_SIZE];
	test_result = new int* [MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; ++i) {
		test_a[i] = new int[MATRIX_SIZE];
		test_b[i] = new int[MATRIX_SIZE];
		test_result[i] = new int[MATRIX_SIZE];
	}
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++) {
			test_a[i][j] = first_matrix[i][j];
			test_b[i][j] = second_matrix[i][j];
			test_result[i][j] = 0;
		}
	MultiplyLocal(test_a, test_b, test_result, MATRIX_SIZE);

	std::cout << "Local result:" << std::endl;
	PrintMatrix(test_result, MATRIX_SIZE);
	return test_result;
}

int main(int argc, char* argv[]) {
	MPI_Init(NULL, NULL);

	GridStructure grid;
	GridSetup(&grid);
	GenerateMatrices();

	int block_size = MATRIX_SIZE / grid.dim;
	int base_row = grid.row * block_size;
	int base_col = grid.col * block_size;

	int** local_a, ** local_b, ** local_c;
	local_a = new int* [MATRIX_SIZE];
	local_b = new int* [MATRIX_SIZE];
	local_c = new int* [MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		local_a[i] = new int[MATRIX_SIZE];
		local_b[i] = new int[MATRIX_SIZE];
		local_c[i] = new int[MATRIX_SIZE];
	}
	for (int i = base_row; i < base_row + block_size; i++)
		for (int j = base_col; j < base_col + block_size; j++) {
			local_a[i - (base_row)][j - (base_col)] = first_matrix[i][j];
			local_b[i - (base_row)][j - (base_col)] = second_matrix[i][j];
			local_c[i - (base_row)][j - (base_col)] = 0;
		}

	if (grid.rank == 0)
	{
		printf("Grid dim %d\n", grid.dim);
	}

	// Blocks until all processes in the communicator have reached this routine.
	MPI_Barrier(grid.grid_comm);

	FoxMultiply(block_size, &grid, local_a, local_b, local_c);

	MPI_Barrier(grid.grid_comm);

	int* result_buff = new int[MATRIX_SIZE * MATRIX_SIZE];
	int* local_buff = new int[block_size * block_size];
	PackMatrix(local_buff, local_c, block_size);

	// Gathers together values from a group of processes
	// int MPI_Gather(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
	//	void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
	MPI_Gather(local_buff, block_size * block_size, MPI_INT, result_buff, block_size * block_size, MPI_INT, 0, grid.grid_comm);
	MPI_Barrier(grid.grid_comm);

	if (grid.rank == 0) {
		int* data = new int[MATRIX_SIZE * MATRIX_SIZE];
		int k = 0;
		for (int bi = 0; bi < grid.dim; bi++)
			for (int bj = 0; bj < grid.dim; bj++)
				for (int i = bi * block_size; i < bi * block_size + block_size; i++)
					for (int j = bj * block_size; j < bj * block_size + block_size; j++) {
						data[i * MATRIX_SIZE + j] = result_buff[k];
						k++;
					}

		std::cout << "Fox result:\n";

		PrintPackedMatrix(data, MATRIX_SIZE);
		int** res = TestSingleThread();
		for (int i = 0; i < MATRIX_SIZE; i++)
			for (int j = 0; j < MATRIX_SIZE; j++)
			{
				if (res[i][j] != data[i * MATRIX_SIZE + j])
				{
					std::cout << "ERROR!" << std::endl;
					exit(1);
				}
			}

		std::cout << "OK" << std::endl;
	}

	MPI_Comm_free(&grid.grid_comm);
	MPI_Comm_free(&grid.col_comm);
	MPI_Comm_free(&grid.row_comm);
	MPI_Finalize();
	exit(0);
}