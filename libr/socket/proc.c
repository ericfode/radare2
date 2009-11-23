/* XXX : move to r_util??? rename method names.. to long? */
/* proc IO is not related to socket io.. this is shitty!! */

#include <r_socket.h>
#include <signal.h>

#if __UNIX__
#include <sys/wait.h>
#endif

R_API struct r_socket_proc_t *r_socket_proc_open(char *const argv[])
{
#if __UNIX__
	struct r_socket_proc_t *sp = MALLOC_STRUCT(struct r_socket_proc_t);
	int flags = O_CLOEXEC; //O_NONBLOCK|O_CLOEXEC;

	if (pipe2(sp->fd0, flags)==-1) {
		perror("pipe");
		free(sp);
		return NULL;
	}
	if (pipe2(sp->fd1, flags)==-1) {
		perror("pipe");
		free(sp);
		return NULL;
	}

	sp->pid = fork();
	switch(sp->pid) {
	case 0:
		close(0);
		dup2(sp->fd0[0], 0);
		close(1);
		dup2(sp->fd1[1], 1);
		execv(argv[0], argv);
		exit(1);
	case -1:
		perror("fork");
		r_socket_proc_close(sp);
		free(sp);
		break;
		//r_socket_block(sp, R_FALSE);
	}
	return sp;
#endif
}

R_API int r_socket_proc_close(struct r_socket_proc_t *sp)
{
#if __UNIX__
	/* this is wrong */
	kill(sp->pid, 9);
	waitpid(sp->pid, NULL, 0); //WNOHANG);
	close(sp->fd0[0]);
	close(sp->fd0[1]);
	//close(sp->fd1[0]);
	close(sp->fd1[1]);
	//sp->fd[0] = -1;
	//sp->fd[1] = -1;
#endif
	return 0;
}