nclude <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>

int MakeTempFd() {
	    do {
		            char name[32];
			            sprintf(name, "tmp_%d.bin", rand());
				            fprintf(stderr, "[debug] trying '%s'...\n", name);
					     
					            int fd = open(name, O_RDWR);
						            if (fd == -1) {
								                if (errno == ENOENT) {
											                // file does not exist
													//                 fd = open(name, O_RDWR | O_CREAT, 0600);
													//                                 fprintf(stderr, "[debug] success\n");
													//                                                 return fd;
													//                                                             }
													//                                                                     } else {
													//                                                                                 close(fd);
													//                                                                                         }
													//                                                                                             } while (1);
													//                                                                                             }
													//
													//                                                                                             int main(){
													//                                                                                             	MakeTempFd()
													//                                                                                             		return 0
													//                                                                                             		}
