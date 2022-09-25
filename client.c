#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdbool.h>
#define size 500
#define PORT 43542

int auto_id = 1000;

int main(){
	char buff[size], to_server[size], username[size], password[size];
	int i, q, p,ch, choice, socket_desc;
	struct sockaddr_in server;    
	   
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);    
	server.sin_family = AF_INET;    
	server.sin_addr.s_addr = INADDR_ANY;    
	server.sin_port = PORT;    
	connect(socket_desc, (void *)(&server), sizeof(server));  
	 
	printf("\n===================Welcome To Railway Ticket Booking System===================\n");
	
	printf("You want to login as user(0) or agent(1) or Admin(2): "); //agent not available
	scanf("%d", &choice);
	choice = htonl(choice);
	write(socket_desc, &choice, sizeof(choice));
	
	
	printf("enter username: ");
	scanf("%s", username);
	 	
	write(socket_desc, username, strlen(username));
	 	
	read(socket_desc, &p, sizeof(p));
	// p=ntohl(p);
	// printf("value of p is %d\n", p);
	if(p==1){
		printf("enter password: ");
		scanf("%s", password);
		 	
		p=htonl(p);
	 	write(socket_desc, password, strlen(password));
	 		
	 	read(socket_desc, &p, sizeof(p));
	 	
	 	if (p!=0){
	 		if (p==10){
	 			printf("\n===================Welcome user: %s===================\n", username);
	 			bool flag=false;
	 			while(1){
	 				printf("choose one option - \n1. View Train lists\n2. View Bookings\n3. Book Tickets\n4. Updated Booking \n5. Cancel Booking \n6. EXIT\n");
	 				scanf("%d", &ch);
					printf("*********************************************************************************\n");
					ch=htonl(ch);
					write(socket_desc, &ch, sizeof(ch));			// write choice to server.
					
					ch=ntohl(ch);
					int train_num, type, status, temp, l, p, q;
					switch(ch){
						case 1:
							printf("--------Train Lists----------\n");
							read(socket_desc, &temp, sizeof(temp));
							printf("total trains = %d\n", temp);
							l = temp;
							for(i=0; i<l; i++){
								read(socket_desc, &temp, sizeof(temp));
								printf("\ntrain_num - %d", temp);
								
								read(socket_desc, &temp, sizeof(temp));
								printf("\nTotal_seats - %d", temp);
								p = temp;
								
								read(socket_desc, &temp, sizeof(temp)); //booked_seats to be returned
								printf("\navailable seats - %d", p-temp);

								read(socket_desc, &temp, sizeof(temp)); 
								if(temp==1)
									printf("\nTrain is Active\n");
								else
									printf("\nTrain is Cancelled\n");
							}
							
							printf("---------------------------------------------------------------------");
							break;
						case 2:
							printf("--------Booking Lists----------\n");
							read(socket_desc, &temp, sizeof(temp));	
							printf("total Booking = %d\n", temp);
							l = temp;
							for(i=0; i<l; i++){
								read(socket_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								printf("\nBooking id - %d", temp);
								
								read(socket_desc, buff , sizeof(buff));
								printf("\nuserName - %s", buff);
								
								read(socket_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								printf("\ntrain_num - %d", temp);
								
								read(socket_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								printf("\nSeats Booked - %d\n", temp);
								
								read(socket_desc, &temp, sizeof(temp));
								temp = htonl(temp);
								printf("\nStatus - %d (0 - cancle)\n", temp);
							}
							
							printf("---------------------------------------------------------------------");
							break;
						case 3:
							printf("Enter train Num to book\n");
							scanf("%d", &p);
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							printf("Enter no. of seats to book\n");
							scanf("%d", &p);
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							p = auto_id;
							auto_id++;
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							read(socket_desc, &temp, sizeof(temp));
							temp = ntohl(temp);
							if (temp==1)
								printf("Booked Successfully\n");
							else if(temp==2)
								printf("Train booking unSuccessful as Train is cancel\n");
							else	
								printf("Booking Unsuccessful\n");
							
							printf("---------------------------------------------------------------------");
							break;
						
						case 4:
							printf("Enter booking id to update\n");
							scanf("%d", &p);
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							printf("Enter no. of seats to inc or dec\n"); 
							scanf("%d", &p);
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							read(socket_desc, &temp, sizeof(temp));
							temp = ntohl(temp);
							if (temp==1)
								printf("Booking updated Successfully\n");
							else	
								printf("Booking update Unsuccessful\n");
								
							printf("---------------------------------------------------------------------");
							break;
						
						case 5:
							printf("Enter booking id to cancel\n");
							scanf("%d", &p);
							write(socket_desc, &p, sizeof(p));
							
							printf("Deleted successfully\n");
							
							printf("---------------------------------------------------------------------");
							break;
						case 6:
							printf("THANK YOU\n");
							flag = true;
							break;
									
					}
					printf("\n");
	 				if (flag)
	 					break;
	 			}
	 		}
	 		else if(p==12){
	 			printf("\n===================Welcome Admin: %s===================\n", username);
	 			bool flag=false;
	 			while(1){
	 				printf("\nchoose one option - \n1. View Train lists\n2. View Bookings\n3. View users\n4. Delete Bookings \n5. Add Users \n6. Add train\n7. Delete User \n8. Cancel train \n9. Resume Cancelled train \n10. Add Agent\n11. EXIT\n");
	 				scanf("%d", &ch);
					
					printf("*********************************************************************************\n");
					ch=htonl(ch);
					write(socket_desc, &ch, sizeof(ch));			// write choice to server.
					
					ch=ntohl(ch);
					int train_num, type, status, temp, l, p, q;
					
					switch(ch){
						case 1:
							printf("--------Train Lists----------\n");
							read(socket_desc, &temp, sizeof(temp));
							printf("total trains = %d\n", temp);
							l = temp;
							for(i=0; i<l; i++){
								read(socket_desc, &temp, sizeof(temp));
								printf("\ntrain_num - %d", temp);
								
								read(socket_desc, &temp, sizeof(temp));
								printf("\nTotal_seats - %d", temp);
								p = temp;
								
								read(socket_desc, &temp, sizeof(temp)); //booked_seats to be returned
								printf("\navailable seats - %d", p-temp);

								read(socket_desc, &temp, sizeof(temp)); 
								if(temp==1)
									printf("\nTrain is Active\n");
								else
									printf("\nTrain is Cancelled\n");
							}
							printf("---------------------------------------------------------------------");
							break;
						
						case 2:
							printf("--------Booking Lists----------\n");
							read(socket_desc, &temp, sizeof(temp));	
							printf("total Booking = %d\n", temp);
							l = temp;
							for(i=0; i<l; i++){
								read(socket_desc, &temp, sizeof(temp));
								printf("\nBooking id - %d", temp);
								
								read(socket_desc, buff , sizeof(buff));
								printf("\nuserName - %s", buff);
								
								read(socket_desc, &temp, sizeof(temp));
								printf("\ntrain_num - %d", temp);
								
								read(socket_desc, &temp, sizeof(temp));
								printf("\nSeats Booked - %d\n", temp);
								
								read(socket_desc, &temp, sizeof(temp));
								printf("\nStatus - %d (0 - cancle)\n", temp);
							}
							
							printf("---------------------------------------------------------------------");
							break;
							
						case 3:
							printf("--------Customers Lists----------\n");
							read(socket_desc, &temp, sizeof(temp));	
							printf("total Customers = %d\n", temp);
							l = temp;
							for(i=0; i<l; i++){
								read(socket_desc, buff, sizeof(buff));
								printf("\nusername - %s", buff);
								
								read(socket_desc, buff , sizeof(buff));
								printf("\npassword - %s", buff);
								
								read(socket_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								printf("\nType - %d", temp);
								
								read(socket_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								printf("\nStatus - %d", temp);
								
								printf("\nBooking ids - ");
								for(p=0;p<10;p++){
									read(socket_desc, &temp, sizeof(temp));
									temp = ntohl(temp);
									if (temp!=0)
										printf("%d ", temp);
								}
								printf("\n");
								
							}
							
							printf("---------------------------------------------------------------------");
							break;
							
						case 4:
							printf("\nEnter booking id to cancel: ");
							scanf("%d", &p);
							p = htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							printf("\nDeleted successfully\n");
							
							printf("---------------------------------------------------------------------");
							break;
						
						case 5:
							printf("\nEnter Username to be added: ");
							char input[size];
							scanf("%s",input);
							write(socket_desc, input, sizeof(input));
							printf("\nenter password of user to add: ");
							scanf("%s", password);
							write(socket_desc, password, sizeof(password));
							
							printf("\nenter user type: (should be zero for normal user): ");
							scanf("%d", &p); //should be 0 here
							p = htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							read(socket_desc, &temp, sizeof(temp));		
							printf("value of temp %d",temp);
							if(temp==0)
								printf("\nUsername already exists\n");
							else
								printf("\nuser added successfully\n");

							printf("---------------------------------------------------------------------");
							break;
						
						case 6:
							printf("Enter a unique train number: ");
							scanf("%d",&p);
							p = htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							printf("Enter a total number of seats: ");
							scanf("%d",&p);
							p = htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							read(socket_desc, &temp, sizeof(temp));		
							temp=ntohl(temp);
							if(temp==0)
								printf("\nTrain number already exits\n");
							else
								printf("\nTrain added successfully\n");

							printf("---------------------------------------------------------------------");
							break;
						//7. Delete User \n8. Cancel train \n9. Resume Cancelled train \n10. EXIT\n");
						case 7:
							printf("Enter the user name to Delete : ");
							scanf("%s", input);
							write(socket_desc, input, sizeof(input));

							printf("Enter the type of user : ");
							scanf("%d",&p);
							write(socket_desc, &p, sizeof(p));

							read(socket_desc, &temp, sizeof(temp));		
							if (temp==1)
								printf("User deleted successfully\n");
							else if (temp==2)
								printf("User already deleted\n");
							else	
								printf("Enter valid User name\n");

							printf("---------------------------------------------------------------------");
							break;

						case 8:
							printf("Enter the train Number to be cancled : ");
							scanf("%d",&p);
							write(socket_desc, &p, sizeof(p));

							read(socket_desc, &temp, sizeof(temp));		
							if (temp==1)
								printf("Train cancelled successfully\n");
							else if (temp==2)
								printf("Train already Cancelled\n");
							else	
								printf("Enter valid train number\n");

							printf("---------------------------------------------------------------------");
							break;

						case 9:
							printf("Enter the train Number to be Resumed : ");
							scanf("%d",&p);
							write(socket_desc, &p, sizeof(p));

							read(socket_desc, &temp, sizeof(temp));		
							if (temp==1)
								printf("Train resumed successfully\n");
							else if (temp==2)
								printf("Train already in Running status\n");
							else	
								printf("Enter valid train number\n");

							
							printf("---------------------------------------------------------------------");
							break;
						
						case 10:
							printf("\nEnter Username to be added: ");
							char inputU[size];
							scanf("%s",inputU);
							write(socket_desc, inputU, sizeof(input));
							printf("\nenter password of user to add: ");
							scanf("%s", password);
							write(socket_desc, password, sizeof(password));
							
							printf("\nenter user type: (should be 1 for agent): ");
							scanf("%d", &p); //should be 0 here
							p = htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							read(socket_desc, &temp, sizeof(temp));		
							printf("value of temp %d",temp);
							if(temp==0)
								printf("\nUsername already exists\n");
							else
								printf("\nAgent added successfully\n");

							printf("---------------------------------------------------------------------");
							break;

						case 11:
							printf("THANK YOU\n");
							flag=true;
							break;
					}
					if (flag)
						break;
	 		}
	 	}
			else if(p==11){
				printf("\n===================Welcome Agent: %s===================\n", username);
	 			bool flag=false;
	 			while(1){
	 				printf("choose one option - \n1. View Train lists\n2. View Bookings\n3. Book Tickets\n4. Updated Booking \n5. Cancel Booking \n6. EXIT\n");
	 				scanf("%d", &ch);
					printf("*********************************************************************************\n");
					ch=htonl(ch);
					write(socket_desc, &ch, sizeof(ch));			// write choice to server.
					
					ch=ntohl(ch);
					int train_num, type, status, temp, l, p, q;
					switch(ch){
						case 1:
							printf("--------Train Lists----------\n");
							read(socket_desc, &temp, sizeof(temp));
							printf("total trains = %d\n", temp);
							l = temp;
							for(i=0; i<l; i++){
								read(socket_desc, &temp, sizeof(temp));
								printf("\ntrain_num - %d", temp);
								
								read(socket_desc, &temp, sizeof(temp));
								printf("\nTotal_seats - %d", temp);
								p = temp;
								
								read(socket_desc, &temp, sizeof(temp)); //booked_seats to be returned
								printf("\navailable seats - %d", p-temp);

								read(socket_desc, &temp, sizeof(temp)); 
								if(temp==1)
									printf("\nTrain is Active\n");
								else
									printf("\nTrain is Cancelled\n");
							}
							
							printf("---------------------------------------------------------------------");
							break;
						case 2:
							printf("--------Booking Lists----------\n");
							read(socket_desc, &temp, sizeof(temp));	
							printf("total Booking = %d\n", temp);
							l = temp;
							for(i=0; i<l; i++){
								read(socket_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								printf("\nBooking id - %d", temp);
								
								read(socket_desc, buff , sizeof(buff));
								printf("\nuserName - %s", buff);
								
								read(socket_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								printf("\ntrain_num - %d", temp);
								
								read(socket_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								printf("\nSeats Booked - %d\n", temp);
								
								read(socket_desc, &temp, sizeof(temp));
								temp = htonl(temp);
								printf("\nStatus - %d (0 - cancle)\n", temp);
							}
							
							printf("---------------------------------------------------------------------");
							break;
						case 3:
							printf("Enter train Num to book\n");
							scanf("%d", &p);
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							printf("Enter no. of seats to book\n");
							scanf("%d", &p);
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							p = auto_id;
							auto_id++;
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							read(socket_desc, &temp, sizeof(temp));
							temp = ntohl(temp);
							if (temp==1)
								printf("Booked Successfully\n");
							else if(temp==2)
								printf("Train booking unSuccessful as Train is cancel\n");
							else	
								printf("Booking Unsuccessful\n");
							
							printf("---------------------------------------------------------------------");
							break;
						
						case 4:
							printf("Enter booking id to update\n");
							scanf("%d", &p);
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							printf("Enter no. of seats to inc or dec\n"); 
							scanf("%d", &p);
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							read(socket_desc, &temp, sizeof(temp));
							temp = ntohl(temp);
							if (temp==1)
								printf("Booking updated Successfully\n");
							else	
								printf("Booking update Unsuccessful\n");
								
							printf("---------------------------------------------------------------------");
							break;
						
						case 5:
							printf("Enter booking id to cancel\n");
							scanf("%d", &p);
							p=htonl(p);
							write(socket_desc, &p, sizeof(p));
							
							printf("Deleted successfully\n");
							
							printf("---------------------------------------------------------------------");
							break;
						case 6:
							printf("THANK YOU\n");
							flag = true;
							break;
									
					}
					printf("\n");
	 				if (flag)
	 					break;
	 			}

			}
	}
	 	else{
			 printf("wrong password\n");
		 }
}
	else if (p==2)
		printf("User Already login. Please logout ann then try \n");
	else{
		printf("wrong username. Pls enter correct one.\n");
	}
}
