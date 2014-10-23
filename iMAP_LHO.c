#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

int main (int argc, char *argv[]){
	int sockfd, fd, exst, rcnt, i, value=1;;
	struct sockaddr_in servaddr;
	char *login=". LOGIN tes@its-sby.edu tesprogjar\r\n",
		 *logout=". LOGOUT\r\n",
		 fetch[300]=". FETCH ",
		 *select=". SELECT INBOX\r\n",
		 *ntr="\r\n",
		 *from,
		 *tmp;
	char perv, buf[2], buf2[1024], username[100], pass[100], sthap[5], halp[5]=". OK";

	// fd=open("index.html", O_RDWR|O_CREAT, 0666);

	sockfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP, TCP_NODELAY);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port=htons(143);
	inet_aton("202.46.129.82", &servaddr.sin_addr);

	int retval = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(retval == -1)
	{
        perror(strerror(errno));
        return (-1);
	}

	// while ((retval=read(sockfd, buf, sizeof(buf)-1)> 0))
	// {
	// 	buf[retval]='\0';
	// 	printf("%s", buf);
	// }
	read(sockfd, buf2, sizeof(buf2));
	printf("Welcome to iMAP LOH! email downloader application :D\n\n");
	printf("-------------------------------------------------------\n");
	// printf("iMAP LOH>> input your username: ");
	// scanf("%s", username);
	// strcat(login, username);
	// printf("iMAP LOH>> input your password: ");
	// scanf("%s", pass);
	// strcat(login, " ");
	// strcat(login, pass);
	
	// strcat(login, ntr);
	// printf("%s", login);
	//login
	write(sockfd, login, strlen(login));
	printf("iMAP LOH>> Loging you in...\n");
	read(sockfd, buf2, sizeof(buf2));
	
	if(buf2[2]=='O' && buf2[3]=='K')
		printf("iMAP LOH>> BOOM! You are now loged in\n");
	
	//select inbox
	write(sockfd, select, strlen(select));
	read(sockfd, buf2, sizeof(buf2));
	strtok(buf2, "\r\n"); 
	strtok(NULL, "\r\n"); 
	tmp=strtok(NULL, "\r\n");
	
	sscanf(tmp, "%*s %d %*s", &exst);
	tmp=strtok(NULL, "\r\n");
	sscanf(tmp, "%*s %d %*s", &rcnt);
	if (rcnt==0)
		printf("iMAP LOH>> There's no new/unread mail and %d old mail:3\n", exst);
	else if (rcnt>0)
		printf("iMAP LOH>> Congrats! %d new/unread mail and %d old mail :3\n", exst, rcnt);
	printf("iMAP LOH>> fetching mail...\n");
	for(i=1; i<=exst; i++)
	{
		char list[200]=". FETCH ";
		sprintf(list, "%s %d", list, i);
		strcat(list, " (FLAGS BODY[HEADER.FIELDS (Subject From)])\r\n");
		write(sockfd, list, strlen(list));
		read(sockfd, buf2, sizeof(buf2));
		printf("--------------------------------\n");
		printf("ID: %d\n", i);
		strtok(buf2, "\r\n");
		tmp=strtok(NULL, "\r\n");
		printf("%s\n", tmp);
		tmp=strtok(NULL, "\r\n");
		printf("%s\n", tmp);

	}
	printf("iMAP LOH>> select ID: ");
	scanf("%d", &i);
	printf("iMAP LOH>> downloading email...\n");
	char filename[200]="email/email";
	sprintf(filename, "%s%d.eml", filename, i);

	fd=open(filename, O_RDWR|O_CREAT, 0666);

	sprintf(fetch, "%s %d", fetch, i);
	strcat(fetch, " BODY[]\r\n");
	write(sockfd, fetch, strlen(fetch));
	bzero(&sthap, sizeof(sthap));
	while ((retval=read(sockfd, buf, sizeof(buf)-1)) > 0)
	{
		
		buf[retval]='\0';
		if (perv=='.' && buf[0]==' '){
			sthap[0]=perv;
			sthap[1]=buf[0];
		}
		else if (perv==' ' && buf[0]=='O'){
			sthap[1]=perv;
			sthap[2]=buf[0];
		}
		else if (perv=='O' && buf[0]=='K'){
			sthap[2]=perv;
			sthap[3]=buf[0];
		}
		if (strcmp(halp,sthap)==0){
			break;
		}
		perv=buf[0];
		write(fd, buf, strlen(buf));
		// printf("%s", buf);
	}
	printf("iMAP LOH>> mail has just downloaded in %s\n", filename);
	write(sockfd, logout, strlen(logout));
	printf("Goodbye\n");
	close(sockfd);
	return 0;

}
