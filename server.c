#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
#include "client_util.h"
#include <stdbool.h>
#define PORT 43542
#define SIZE 500

int main(){
	if(findSize()==-1)
	{
		struct customer c;
		printf("create admin\nenter username:");
		scanf("%s", c.username);
		printf("enter password:");
		scanf("%s", c.password);
		c.type = 2;
		c.status = 1;
		int q=0;
		for(q=0; q<10; q++)
			c.booking_id[q] = 0;
		addAdmin(c);
	}
	
	struct customer c1;
	
	struct sockaddr_in server, client;    
	int socket_desc, SIZE_client, client_desc,temp, acno;    
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);    
	server.sin_family = AF_INET;    
	server.sin_addr.s_addr = INADDR_ANY;    
	server.sin_port = PORT;    
	
	char user[SIZE], pass[SIZE], sample[SIZE], sample2[SIZE];
	
	if(bind(socket_desc, (void *)(&server), sizeof(server)) < 0)    
	{        
		perror("Error on binding:");        
		exit(EXIT_FAILURE);    
	}  
	
	listen(socket_desc, 5);    
	while(1)
	{
		SIZE_client = sizeof(client);        
		if((client_desc = accept(socket_desc, (struct sockaddr*)&client,&SIZE_client)) < 0) 
		{            
			perror("Error on accept:");            
			exit(EXIT_FAILURE);        
		}
		if (!fork()){
			while(1){
				int choice, buff;
				read(client_desc, &choice, sizeof(choice));
				choice = ntohl(choice); 
				
				read(client_desc, user, sizeof(user));  // reading username
				
				if(choice==0 || choice==2)
					buff = isUserValid(user, choice);   		// username validation.
				else
					buff = isUserAgentValid(user);	// for joint user. TO BE ADD
				
				// buff=htonl(buff);
				printf("\nvalue of buff %d",buff);
				write(client_desc, &buff, sizeof(buff));   // sending client whether the user is valid(1) or not(0)
				 
				/***PASSWORD***/
				read(client_desc, pass, sizeof(pass));	// reading password
				
				if(choice==0 || choice==2)
					buff = isPasswordValid(user,pass, choice);    // password validation and finding the type of user it is. 0 -> not valid, 10-> normal user, 11->joint user, 12->admin
				else
					buff = isAgentPasswordValid(user,pass); 	
								
				
				printf("buff is %d ", buff);
				write(client_desc, &buff, sizeof(int));	// sending validation information to client.
				
				if (buff==10){
					updateCustomerStatus(user, choice, 2); //2 indicates user is in login
					bool flag=false;
					while(1){
						int switchChoice;
						read(client_desc, &switchChoice, sizeof(switchChoice));	// read choice inside admin
						switchChoice=ntohl(switchChoice);
						
						int b, train_no, booking_id, temp, len, q, seat;
						
						switch(switchChoice){
							case 1:
								;
								struct train arr[20];
								len = getTrainDetails(arr);
								write(client_desc, &len, sizeof(len));
								
								for(q=0;q<len;q++){
									temp = arr[q].train_num;
									write(client_desc, &temp, sizeof(temp));

									temp = arr[q].total_seats;
									write(client_desc, &temp, sizeof(temp));

									temp = arr[q].booked_seats; 
									write(client_desc, &temp, sizeof(temp));

									temp = arr[q].status; 
									write(client_desc, &temp, sizeof(temp));
								}
								break;
							
							case 2:
								;
								struct booking book_arr[20];
								len = getBookingDetailsByUsername(book_arr, user);
								write(client_desc, &len, sizeof(len));
								
								for(q=0;q<len;q++){
									temp = book_arr[q].booking_id;
									temp = htonl(temp);
									write(client_desc, &temp, sizeof(temp));

									strcpy(sample, book_arr[q].username);
									write(client_desc, &sample, sizeof(sample));

									temp = book_arr[q].train_num;
									temp = htonl(temp);
									write(client_desc, &temp, sizeof(temp));

									temp = book_arr[q].seats_booked;
									temp = htonl(temp);
									write(client_desc, &temp, sizeof(temp));

									temp = book_arr[q].status;
									temp = htonl(temp);
									write(client_desc, &temp, sizeof(temp));
								}
								break;

							case 3:
								read(client_desc, &train_no, sizeof(train_no));
								train_no = ntohl(train_no);

								read(client_desc, &seat, sizeof(seat));
								seat = ntohl(seat);

								read(client_desc, &booking_id, sizeof(booking_id));
								booking_id = ntohl(booking_id);
								
								struct booking new_booking;
								new_booking.train_num = train_no;
								new_booking.booking_id = booking_id;
								new_booking.seats_booked = seat;
								strcpy(new_booking.username, user);
								new_booking.status = 1;

								temp = addBooking(new_booking, user);
								temp = htonl(temp);
								write(client_desc, &temp, sizeof(temp));

								break;

							case 4:
								read(client_desc, &booking_id, sizeof(booking_id));
								booking_id = ntohl(booking_id);

								read(client_desc, &seat, sizeof(seat));
								seat = ntohl(seat);

								temp = updateBooking(booking_id, seat, user);
								temp = htonl(temp);
								write(client_desc, &temp, sizeof(temp));

								break;

							case 5:
								read(client_desc, &booking_id, sizeof(booking_id));
								deleteBookingByUser(booking_id, user);
								break;

							case 6:
								flag = true;
								updateCustomerStatus(user, choice, 1); //1 indicates user is logged out
								break;
						}
						if(flag)
							break;
					}
				}
				else if(buff==12){
					updateCustomerStatus(user, choice, 2); //2 indicates admin is in login
					bool flag=false;
					while(1){
						int switchChoice;
						read(client_desc, &switchChoice, sizeof(switchChoice));	// read choice inside admin
						switchChoice=ntohl(switchChoice);
						
						int b, train_no, booking_id, temp, len, q, seat;

						switch(switchChoice){
							case 1:
								printf("here");
								struct train arr[20];
								len = getTrainDetails(arr);
								write(client_desc, &len, sizeof(len));
								
								for(q=0;q<len;q++){
									temp = arr[q].train_num;
									write(client_desc, &temp, sizeof(temp));

									temp = arr[q].total_seats;
									write(client_desc, &temp, sizeof(temp));

									temp = arr[q].booked_seats; 
									write(client_desc, &temp, sizeof(temp));

									temp = arr[q].status; 
									write(client_desc, &temp, sizeof(temp));
								}
								break;

							case 2:
								;
								struct booking book_arr[20];
								len = getBookingDetails(book_arr);
								write(client_desc, &len, sizeof(len));
								
								for(q=0;q<len;q++){
									temp = book_arr[q].booking_id;
									write(client_desc, &temp, sizeof(temp));

									strcpy(sample, book_arr[q].username);
									write(client_desc, &sample, sizeof(sample));

									temp = book_arr[q].train_num;
									write(client_desc, &temp, sizeof(temp));

									temp = book_arr[q].seats_booked;
									write(client_desc, &temp, sizeof(temp));

									temp = book_arr[q].status;
									write(client_desc, &temp, sizeof(temp));
								}
								break;

							case 3:
								;
								struct customer cust_arr[20];
								len = getUserDetails(cust_arr);
								write(client_desc, &len, sizeof(len));
								
								for(q=0;q<len;q++){
									strcpy(sample, cust_arr[q].username);
									write(client_desc, &sample, sizeof(sample));

									strcpy(sample, cust_arr[q].password);
									write(client_desc, &sample, sizeof(sample));

									temp = cust_arr[q].type;
									temp = htonl(temp);
									write(client_desc, &temp, sizeof(temp));

									temp = cust_arr[q].status;
									temp = htonl(temp);
									write(client_desc, &temp, sizeof(temp));

									for(b=0; b<10; b++){
										temp = cust_arr[q].booking_id[b];
										temp = htonl(temp);
										write(client_desc, &temp, sizeof(temp));
									}

								}
								break;
							
							case 4:
								read(client_desc, &booking_id, sizeof(booking_id));
								booking_id = ntohl(booking_id);

								deleteBooking(booking_id);
								break;
							
							case 5:
								printf("Add user");
								struct customer c;
								read(client_desc, c.username, sizeof(c.username));

								read(client_desc, c.password, sizeof(c.password));

								read(client_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								c.type = temp;

								c.status = 1;

								for(b=0;b<10;b++)
									c.booking_id[b]  = 0;
								
								temp = addUser(c);
								printf("temp is %d\n",temp);
								write(client_desc, &temp, sizeof(temp));

								break;

							case 6:
								;
								struct train t;
								read(client_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								t.train_num = temp;

								read(client_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								t.total_seats = temp;

								t.booked_seats=0;
								t.status=1;
								
								temp = addTrain(t);
								temp = htonl(temp);
								write(client_desc, &temp, sizeof(temp));
								break;
							
						//7. Delete User \n8. Cancel train \n9. Resume Cancelled train \n10. EXIT\n");
							case 7:
								read(client_desc, &sample, sizeof(sample));
								read(client_desc, &temp, sizeof(temp));

								temp = deleteCustomer(sample, temp);
								write(client_desc, &temp, sizeof(temp));
								break;

							case 8:
								read(client_desc, &train_no, sizeof(train_no));

								temp = deleteTrain(train_no);
								write(client_desc, &temp, sizeof(temp));
								break;
							
							case 9:
								read(client_desc, &train_no, sizeof(train_no));

								temp = resumeTrain(train_no);
								write(client_desc, &temp, sizeof(temp));
								break;
							
							case 10:
								printf("Add Agent");
								struct agent a;
								read(client_desc, a.username, sizeof(a.username));

								read(client_desc, a.password, sizeof(a.password));

								read(client_desc, &temp, sizeof(temp));
								temp = ntohl(temp);
								a.type = temp;

								a.status = 1;

								for(b=0;b<10;b++)
									a.booking_id[b]  = 0;
								
								temp = addAgent(a);
								printf("temp is %d\n",temp);
								write(client_desc, &temp, sizeof(temp));

								break;

							case 11:
								flag = true;
								updateCustomerStatus(user, choice, 1); //1 indicates user is logged out
								break;

						}
						if(flag)
							break;
					}

				}
				else if (buff==11){
					bool flag=false;
					while(1){
						int switchChoice;
						read(client_desc, &switchChoice, sizeof(switchChoice));	// read choice inside admin
						switchChoice=ntohl(switchChoice);
						
						int b, train_no, booking_id, temp, len, q, seat;
						
						switch(switchChoice){
							case 1:
								;
								struct train arr[20];
								len = getTrainDetails(arr);
								write(client_desc, &len, sizeof(len));
								
								for(q=0;q<len;q++){
									temp = arr[q].train_num;
									write(client_desc, &temp, sizeof(temp));

									temp = arr[q].total_seats;
									write(client_desc, &temp, sizeof(temp));

									temp = arr[q].booked_seats; 
									write(client_desc, &temp, sizeof(temp));

									temp = arr[q].status; 
									write(client_desc, &temp, sizeof(temp));
								}
								break;
							
							case 2:
								;
								struct booking book_arr[20];
								len = getBookingDetailsByUsername(book_arr, user);
								write(client_desc, &len, sizeof(len));
								
								for(q=0;q<len;q++){
									temp = book_arr[q].booking_id;
									temp = htonl(temp);
									write(client_desc, &temp, sizeof(temp));

									strcpy(sample, book_arr[q].username);
									write(client_desc, &sample, sizeof(sample));

									temp = book_arr[q].train_num;
									temp = htonl(temp);
									write(client_desc, &temp, sizeof(temp));

									temp = book_arr[q].seats_booked;
									temp = htonl(temp);
									write(client_desc, &temp, sizeof(temp));

									temp = book_arr[q].status;
									temp = htonl(temp);
									write(client_desc, &temp, sizeof(temp));
								}
								break;

							case 3:
								read(client_desc, &train_no, sizeof(train_no));
								train_no = ntohl(train_no);

								read(client_desc, &seat, sizeof(seat));
								seat = ntohl(seat);

								read(client_desc, &booking_id, sizeof(booking_id));
								booking_id = ntohl(booking_id);

								struct booking new_booking;
								new_booking.train_num = train_no;
								new_booking.booking_id = booking_id;
								new_booking.seats_booked = seat;
								strcpy(new_booking.username, user);
								new_booking.status = 1;

								temp = addBookingAgent(new_booking, user);
								temp = htonl(temp);
								write(client_desc, &temp, sizeof(temp));

								break;

							case 4:
								read(client_desc, &booking_id, sizeof(booking_id));
								booking_id = ntohl(booking_id);

								read(client_desc, &seat, sizeof(seat));
								seat = ntohl(seat);

								temp = updateBooking(booking_id, seat, user);
								temp = htonl(temp);
								write(client_desc, &temp, sizeof(temp));

								break;

							case 5:
								read(client_desc, &booking_id, sizeof(booking_id));
								booking_id = ntohl(booking_id);

								deleteBookingByUser(booking_id, user);
								break;

							case 6:
								flag = true;
								break;
						}
						if(flag)
							break;
					}
			}
			
			
		} 
	}  

}}
