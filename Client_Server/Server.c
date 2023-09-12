#include"Header.h"
void client_info(struct sockaddr_in client_info)
{
printf("client port address:
%d\n",client_info.sin_port);
printf("client ip address:
%s\n",inet_ntoa(client_info.sin_addr));

}
int main(int argc,char **argv)
{
if(argc!=3)
{
printf("usage: ./server server_port

server_ip_address\n");

return 0;
}
int sfd,len,nsfd;
struct sockaddr_in server_id,client_id;
/********** socket () ***************/
sfd=socket(AF_INET,SOCK_STREAM,0);
if(sfd<0)
{
perror("Socket");
return 0;
}
perror("Socket");
/*********** bind() ***************/
server_id.sin_family=AF_INET;
server_id.sin_port=htons(atoi(argv[1]));
server_id.sin_addr.s_addr=inet_addr(argv[2]);

len=sizeof(server_id);
if(bind(sfd,(struct sockaddr*)
&server_id,len)<0)
{
perror("Bind");
return 0;
}
perror("Bind");

/************* listen **************/
if(listen(sfd,5)<0)
{
perror("Listen");
return 0;
}

perror("Listen");
/************ accept ****************/
printf("before accept client request...(waiting
for client)\n");
nsfd=accept(sfd,(struct
sockaddr*)&client_id,&len);
if(nsfd<0)
{
perror("Accept");
return 0;
}
printf("client request Accepted...\n");
client_info(client_id);
char s[20];
read(nsfd,s,sizeof(s)); // reading data from
client
printf("client data: %s\n",s);

close(sfd);
close(nsfd);

}
