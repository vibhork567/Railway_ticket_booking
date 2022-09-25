#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#define size 500

struct customer{
	char username[size];
	char password[size];
	int booking_id[10];
	int type;
	int status;
};

struct train{
	int train_num;
	int total_seats;
	int booked_seats;
	int status;
};

struct booking{
	int booking_id;
	char username[size];
	int train_num;
	int seats_booked;
	int status;
};

struct agent{
	char username[size];
	char password[size];
	int booking_id[10];
	int type;
	int status;
};

int agent(struct agent agnt){
	int fd = open("agent.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	struct agent a1;
	
	int flag=false;
	while(read(fd, &a1, sizeof(a1))>0)
	{
		if(strcmp(a1.username, agnt.username)==0)
		 {
			flag=true;
			break;
		}
	}
	if(!flag)		// username doesnt exist
	{
		printf("\nunder not flag\n");
		struct flock lock;
		lock.l_type = F_WRLCK;	
		lock.l_whence = SEEK_CUR;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
		write(fd, &agnt, sizeof(agnt));
		lock.l_type = F_UNLCK;			// unlocking.
		fcntl(fd, F_SETLK, &lock);		// unlocking.
		
		close(fd);
		
		// -----------locking and writing into customer file-------------------
		
		return 1;
	}
	else{		// username exists.
		close(fd);
		return 0;}
	close(fd);
}

int addAgent(struct agent cstmr){
	int fd=open("agent.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	struct agent c1;
	
	int flag=false;
	while(read(fd, &c1, sizeof(c1))>0)
	{
		if( strcmp(c1.username, cstmr.username)==0)
		{
			flag=true;
			break;
		}
	}
	if(!flag)		// username doesnt exist
	{
		printf("\nunder not flag\n");
		struct flock lock;
		lock.l_type = F_WRLCK;	
		lock.l_whence = SEEK_CUR;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
		write(fd, &cstmr, sizeof(cstmr));
		lock.l_type = F_UNLCK;			// unlocking.
		fcntl(fd, F_SETLK, &lock);		// unlocking.
		
		close(fd);
		
		// -----------locking and writing into customer file-------------------
		
		return 1;
	}
	else{		// username exists.
		close(fd);
		return 0;}
	close(fd);
}

int addUser(struct customer cstmr){
	int fd=open("customer.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	struct customer c1;
	
	int flag=false;
	while(read(fd, &c1, sizeof(c1))>0)
	{
		if( strcmp(c1.username, cstmr.username)==0 && c1.status==1)
		{
			flag=true;
			break;
		}
	}
	if(!flag)		// username doesnt exist
	{
		printf("\nunder not flag\n");
		struct flock lock;
		lock.l_type = F_WRLCK;	
		lock.l_whence = SEEK_CUR;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
		write(fd, &cstmr, sizeof(cstmr));
		lock.l_type = F_UNLCK;			// unlocking.
		fcntl(fd, F_SETLK, &lock);		// unlocking.
		
		close(fd);
		
		// -----------locking and writing into customer file-------------------
		
		return 1;
	}
	else{		// username exists.
		close(fd);
		return 0;}
	close(fd);
}

int addTrain(struct train trn){
	int fd=open("train.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	struct train t;
	
	int flag=false;
	while(read(fd, &t, sizeof(t))>0)
	{
		if(t.train_num - trn.train_num==0)
		{
			flag=true;
		}
	}
	if(!flag)		// train doesnt exist
	{
		struct flock lock;
		lock.l_type = F_WRLCK;	
		lock.l_whence = SEEK_CUR;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
		write(fd, &trn, sizeof(trn));
		lock.l_type = F_UNLCK;			// unlocking.
		fcntl(fd, F_SETLK, &lock);		// unlocking.
		
		close(fd);
		
		// -----------locking and writing into customer file-------------------
		
		return 1;
	}
	else		// train exists.
		return 0;
	close(fd);
}

int addBooking(struct booking bking, char *username){
	if (bking.seats_booked<=0)
		return 0; //Booking not possible due to negative number
	int fd=open("booking.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	/******counting total number of bookings*************/
	int count=0;
	struct booking b1;
	while(read(fd, &b1, sizeof(b1))>0)
		count++;

	bking.booking_id = 1000+count+1;
	close(fd);
	/****User details to be checked and modified*************/
	int fd3 = open("customer.txt", O_RDWR, 0766); //for user i.e customer
	struct customer c1;
	struct customer c;
	int i=1;
	int x, y;
	while(read(fd3, &c1, sizeof(c1))>0)
	{
		if(strcmp(c1.username, username)==0)
		{	
			bool flagId = false;
			for(x=0; x<10; x++){
				if (c1.booking_id[x]==0){
					flagId=true;
					break;
				}
			}
			if(!flagId)
				return 0; //Booking Not possible due to access bookings
		
			strcpy(c.username, c1.username);
			strcpy(c.password, c1.password);
			c.type = c1.type;
			c.status = c1.status;
			
			for(y=0; y<10;y++){
				c.booking_id[y] = c1.booking_id[y];
			}
			c.booking_id[x] = bking.booking_id;
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*1048)+1;
			lock.l_len = 1048;
			lock.l_pid = getpid();
		
			fcntl(fd3, F_SETLKW, &lock);
				
			lseek(fd3, ((i-1) * 1048), SEEK_SET);
				
			write(fd3, &c, 1048);
				
			lock.l_type = F_UNLCK;
			fcntl(fd3, F_SETLK, &lock);
			break;

		}
		i++;
	}
	close(fd3);
	/********************************************************/
	
	
	/****Train num to be checked and seats to be uppdated****/
	int fd2 = open("train.txt", O_CREAT | O_RDWR, 0766); //for Train
	struct train t1, t;
	i=1;
	
	int flag=false;
	while(read(fd2, &t1, sizeof(t))>0)
	{
		if((t1.train_num - bking.train_num==0) && (t1.total_seats-t1.booked_seats>=bking.seats_booked) && t1.status==1)
		{
			flag = true;		
			t.train_num = t1.train_num;
			t.total_seats = t1.total_seats;
			t.booked_seats = t1.booked_seats + bking.seats_booked;
			t.status = t1.status;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*16)+1;
			lock.l_len = 16;
			lock.l_pid = getpid();
		
			fcntl(fd2, F_SETLKW, &lock);
				
			lseek(fd2, ((i-1) * 16), SEEK_SET);
				
			write(fd2, &t, 16);
				
			lock.l_type = F_UNLCK;
			fcntl(fd2, F_SETLK, &lock);
			break;

		}
		if((t1.train_num - bking.train_num==0)&& t1.status==0)
		{
			close(fd);
			close(fd2);
			return 2;
		}
		i++;
	}
	/*******Train checked and seats checked **************/
	fd=open("booking.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	if(flag){
		struct flock lock;
		lock.l_type = F_WRLCK;	
		lock.l_whence = SEEK_CUR;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
		write(fd, &bking, sizeof(bking));
		lock.l_type = F_UNLCK;			// unlocking.
		fcntl(fd, F_SETLK, &lock);		// unlocking.
		
		close(fd);
		close(fd2);
		return 1; //boooked successfully
	}
	else{
		close(fd);
		close(fd2);
		return 0; //booking unsuccessful bcoz of seats unavailability or train doesn't exits
	}
}

int addBookingAgent(struct booking bking, char *username){
	if (bking.seats_booked<=0)
		return 0;
	int fd=open("booking.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	/******counting total number of bookings*************/
	int count=0;
	struct booking b1;
	while(read(fd, &b1, sizeof(b1))>0)
		count++;

	bking.booking_id = 1000+count+1;
	close(fd);
	/****User details to be checked and modified*************/
	int fd3 = open("agent.txt", O_RDWR, 0766); //for user i.e customer
	struct customer c1;
	struct customer c;
	int i=1;
	int x, y;
	while(read(fd3, &c1, sizeof(c1))>0)
	{
		if(strcmp(c1.username, username)==0)
		{	
			bool flagId = false;
			for(x=0; x<10; x++){
				if (c1.booking_id[x]==0){
					flagId=true;
					break;
				}
			}
			if(!flagId)
				return 0; //Booking Not possible due to access bookings
		
			strcpy(c.username, c1.username);
			strcpy(c.password, c1.password);
			c.type = c1.type;
			c.status = c1.status;
			
			for(y=0; y<10;y++){
				c.booking_id[y] = c1.booking_id[y];
			}
			c.booking_id[x] = bking.booking_id;
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*1048)+1;
			lock.l_len = 1048;
			lock.l_pid = getpid();
		
			fcntl(fd3, F_SETLKW, &lock);
				
			lseek(fd3, ((i-1) * 1048), SEEK_SET);
				
			write(fd3, &c, 1048);
				
			lock.l_type = F_UNLCK;
			fcntl(fd3, F_SETLK, &lock);
			break;

		}
		i++;
	}
	close(fd3);
	/********************************************************/
	
	
	/****Train num to be checked and seats to be uppdated****/
	int fd2 = open("train.txt", O_CREAT | O_RDWR, 0766); //for Train
	struct train t1, t;
	i=1;
	
	int flag=false;
	while(read(fd2, &t1, sizeof(t))>0)
	{
		if((t1.train_num - bking.train_num==0) && (t1.total_seats-t1.booked_seats>=bking.seats_booked) && t1.status==1)
		{
			flag = true;		
			t.train_num = t1.train_num;
			t.total_seats = t1.total_seats;
			t.booked_seats = t1.booked_seats + bking.seats_booked;
			t.status = t1.status;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*16)+1;
			lock.l_len = 16;
			lock.l_pid = getpid();
		
			fcntl(fd2, F_SETLKW, &lock);
				
			lseek(fd2, ((i-1) * 16), SEEK_SET);
				
			write(fd2, &t, 16);
				
			lock.l_type = F_UNLCK;
			fcntl(fd2, F_SETLK, &lock);
			break;

		}
		if((t1.train_num - bking.train_num==0)&& t1.status==0)
		{
			close(fd);
			close(fd2);
			return 2;
		}
		i++;
	}
	/*******Train checked and seats checked **************/
	fd=open("booking.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	if(flag){
		struct flock lock;
		lock.l_type = F_WRLCK;	
		lock.l_whence = SEEK_CUR;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
		write(fd, &bking, sizeof(bking));
		lock.l_type = F_UNLCK;			// unlocking.
		fcntl(fd, F_SETLK, &lock);		// unlocking.
		
		close(fd);
		close(fd2);
		return 1; //boooked successfully
	}
	else{
		close(fd);
		close(fd2);
		return 0; //booking unsuccessful bcoz of seats unavailability or train doesn't exits
	}
}

//*************ADDITION ENDS*****************************

int getUserDetails(struct customer *arr)
{
	int fd=open("customer.txt", O_RDONLY, 0766);
	
	printf("getUserDetails\n");
	
	struct customer c;
	int i=0;
	
	while(read(fd, &c, sizeof(c))>0)
	{
		if(c.status==1)
			printf("\n%s\t%s\t%d", c.username, c.password,c.type);
			
		arr[i] = c;
		i++;
	}
	close(fd);
	return i;
}

int getBookingDetails(struct booking *arr)
{
	int fd=open("booking.txt", O_RDONLY, 0766);
	
	printf("Booking Details\n");
	struct booking b;
	int i=0;
	
	while(read(fd, &b, sizeof(b))>0)
	{
		if(b.status==1)		
			printf("\n%d\t%s\t%d\t%d", b.booking_id, b.username, b.train_num, b.seats_booked);
		arr[i] = b;
		i++;
	}
	close(fd);
	return i;
}

int getTrainDetails(struct train* arr)
{
	int fd=open("train.txt", O_RDONLY, 0766);
	
	printf("Train Details\n");
	struct train t;
	int i=0;
	
	while(read(fd, &t, sizeof(t))>0)
	{
		if(t.status==1)		
			printf("\n%d\t%d\t%d", t.train_num, t.total_seats, t.booked_seats);
		
		arr[i] = t;
		i++;
	}
	close(fd);
	return i;
}

int getBookingDetailsByUsername(struct booking *arr, char *username){
	int fd=open("booking.txt", O_RDONLY, 0766);
	
	printf("Booking Details\n");
	struct booking b;
	int i=0;
	
	while(read(fd, &b, sizeof(b))>0)
	{
		if(strcmp(b.username, username)==0){		
			printf("\n%d\t%s\t%d\t%d\t%d", b.booking_id, b.username, b.train_num, b.seats_booked, b.status);
			arr[i] = b;
			i++;
			}
			
		
	}
	close(fd);
	return i;
}


/************VALIDATION************/
int isUserValid(char *name, int choice)
{
	int fd=open("customer.txt", O_RDONLY, 0766);
		
	struct customer c;
		
	while(read(fd, &c, sizeof(c))>0)
	{		
		if((c.type==choice) && (strcmp(c.username, name) == 0) && c.status==1)
			return 1;
		else if((c.type==choice) && (strcmp(c.username, name) == 0) && c.status==2)
			return 2;
	}
	close(fd);
	return 0;
}


int isUserAgentValid(char *name)
{
	int fd=open("agent.txt", O_RDONLY, 0766);
		
	struct agent a;
		
	while(read(fd, &a, sizeof(a))>0)
	{		
		if(strcmp(a.username, name) == 0 && a.status==1)
			return 1;
	}
	close(fd);
	return 0;
}

int isPasswordValid(char *name, char *pass, int choice)
{
	int fd=open("customer.txt", O_RDONLY, 0766);
	int value;
	
	struct customer c;
	
	while(read(fd, &c, sizeof(c))>0)
	{		
		if( (c.type==choice) && strcmp(c.username,name)==0 && strncmp(c.password, pass, strlen(c.password))==0)
		{
			value = c.type + 10;
			printf("value of value %d\n", value);
			return value;
		}
	}
	close(fd);
	return (0);
}

int isAgentPasswordValid(char *name, char *pass)
{
	int fd=open("agent.txt", O_RDONLY, 0766);
	int value;
	
	struct agent a;
	
	while(read(fd, &a, sizeof(a))>0)
	{		
		if(strcmp(a.username, name) == 0 && strcmp(a.password, pass)==0)
		{
			value = a.type + 10;
			printf("value of value %d\n", value);
			return value;
		}
	}
	close(fd);
	return (0);
}

/********VALIDATION ENDS**************/

long int findSize(){
	struct stat st; 
    
    	char *file_name = "customer.txt"; 
     
    	if(stat(file_name,&st)==0)
		return (st.st_size);
    	else
		return -1;
}

int addAdmin(struct customer cstmr) 
{
	int fd=open("customer.txt", O_CREAT | O_RDWR | O_APPEND, 0766);
	write(fd, &cstmr, sizeof(cstmr));
	close(fd);
}

//******************************SEARCHING***********************************

struct train searchTrainDetails(int train_num)
{
	int fd=open("train.txt", O_RDONLY, 0766);
	
	struct flock lock;
		
	lock.l_type = F_RDLCK;	
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
		
	fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
	
	
	struct train t;
	bool flag=false;
	while(read(fd, &t, sizeof(t))>0)
	{		
		if(t.train_num == train_num)
		{
			flag=true;
			break;
		}
	}
	
	lock.l_type = F_UNLCK;			// unlocking.
	fcntl(fd, F_SETLK, &lock);		// unlocking.
	
	if(!flag)
		return t;
	else
	{
		t.train_num = -10;
		return t;	
		
	}
	close(fd);
}

struct booking searchBookingDetails(int booking_id)
{
	int fd=open("booking.txt", O_RDONLY, 0766);
	
	struct flock lock;
		
	lock.l_type = F_RDLCK;	
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
		
	fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
	
	
	struct booking b;
	bool flag=false;
	while(read(fd, &b, sizeof(b))>0)
	{		
		if(b.booking_id == booking_id)
		{
			flag=true;
			break;
		}
	}
	
	lock.l_type = F_UNLCK;			// unlocking.
	fcntl(fd, F_SETLK, &lock);		// unlocking.
	
	if(!flag)
		return b;
	else
	{
		b.booking_id = -10;
		return b;	
		
	}
	close(fd);
}

struct customer viewDetailsByName(char *name)
{
	int fd=open("customer.txt", O_RDONLY, 0766);
	
	struct flock lock;
		
	lock.l_type = F_RDLCK;	
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
		
	fcntl(fd, F_SETLKW, &lock);		// locking.  -> here on critical section.
	
	
	struct customer c;
	while(read(fd, &c, sizeof(c))>0)
	{		
		if(strcmp(c.username, name) == 0)
		{
			
			lock.l_type = F_UNLCK;			// unlocking.
			fcntl(fd, F_SETLK, &lock);		// unlocking.
			return c;
		}
	}
	lock.l_type = F_UNLCK;			// unlocking.
	fcntl(fd, F_SETLK, &lock);		// unlocking.

	c.type=-10; //not found
	return c; 
	close(fd);
}


//*************************************DELETING*********************************



void deleteBooking(int booking_id)
{
	int fd=open("booking.txt", O_RDWR, 0766);
	int i=1;
	
	struct booking b1, b;
	
	while(read(fd, &b1, sizeof(b1))>0)
	{		
		if(booking_id - b1.booking_id==0)
		{
			
			b.booking_id = b1.booking_id;
			b.train_num = b1.train_num;
			strcpy(b.username, b1.username);
			b.seats_booked = b1.seats_booked;
			b.status = 0;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*516)+1;
			lock.l_len = 516;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 516), SEEK_SET);
				
			write(fd, &b, 516);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
			
			//**** train seats to be modified *****//
			int fd2=open("train.txt", O_RDWR, 0766);
			int i=1;
			
			struct train t1, t;
			
			while(read(fd2, &t1, sizeof(t1))>0)
			{		
				if(b.train_num - t1.train_num==0)
				{
					
					t.train_num = t1.train_num;
					t.total_seats = t1.total_seats;
					t.booked_seats = t1.booked_seats - b.seats_booked;
					t.status = t1.status;
						
					
					struct flock lock;
					lock.l_type = F_WRLCK;
					lock.l_whence = SEEK_SET;
					lock.l_start = ((i-1)*16)+1;
					lock.l_len = 16;
					lock.l_pid = getpid();
				
					fcntl(fd2, F_SETLKW, &lock);
						
					lseek(fd2, ((i-1) * 16), SEEK_SET);
						
					write(fd2, &t, 16);
						
					lock.l_type = F_UNLCK;
					fcntl(fd2, F_SETLK, &lock);

				}
				i++;
			}
			
			close(fd2);
			//*******train seat modified

		}
		i++;
	}
	
	close(fd);
}

void deleteBookingByUser(int booking_id, char* username)
{
	int fd=open("booking.txt", O_RDWR, 0766);
	int i=1;
	
	struct booking b1, b;
	
	while(read(fd, &b1, sizeof(b1))>0)
	{		
		if((booking_id - b1.booking_id==0) && strcmp(b1.username, username)==0)
		{
			
			b.booking_id = b1.booking_id;
			b.train_num = b1.train_num;
			strcpy(b.username, b1.username);
			b.seats_booked = b1.seats_booked;
			b.status = 0;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*516)+1;
			lock.l_len = 516;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 516), SEEK_SET);
				
			write(fd, &b, 516);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
			
			//**** train seats to be modified *****//
			int fd2=open("train.txt", O_RDWR, 0766);
			int i=1;
			
			struct train t1, t;
			
			while(read(fd2, &t1, sizeof(t1))>0)
			{		
				if(b.train_num - t1.train_num==0)
				{
					
					t.train_num = t1.train_num;
					t.total_seats = t1.total_seats;
					t.booked_seats = t1.booked_seats - b.seats_booked;
					t.status = t1.status;
						
					
					struct flock lock;
					lock.l_type = F_WRLCK;
					lock.l_whence = SEEK_SET;
					lock.l_start = ((i-1)*16)+1;
					lock.l_len = 16;
					lock.l_pid = getpid();
				
					fcntl(fd2, F_SETLKW, &lock);
						
					lseek(fd2, ((i-1) * 16), SEEK_SET);
						
					write(fd2, &t, 16);
						
					lock.l_type = F_UNLCK;
					fcntl(fd2, F_SETLK, &lock);

				}
				i++;
			}
			
			close(fd2);
			//*******train seat modified

		}
		i++;
	}
	
	close(fd);
}

void deleteBookingBasedTrain(int train_num)
{
	int fd=open("booking.txt", O_RDWR, 0766);
	int i=1;
	
	struct booking b1, b;
	
	while(read(fd, &b1, sizeof(b1))>0)
	{		
		if(train_num - b1.train_num==0)
		{
			
			b.booking_id = b1.booking_id;
			b.train_num = b1.train_num;
			strcpy(b.username, b1.username);
			b.seats_booked = b1.seats_booked;
			b.status = 0;
				
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*516)+1;
			lock.l_len = 516;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 516), SEEK_SET);
				
			write(fd, &b, 516);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
			

		}
		i++;
	}
	
	close(fd);
}

int deleteCustomer(char *username, int type)
{
	int fd=open("customer.txt", O_RDWR, 0766);
	int i=1;
	
	struct customer c1, c;
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		if(strncmp(c1.username, username, strlen(c1.username))==0 && c1.type==type &&c1.status==1)
		{
			
			strcpy(c.username, c1.username);		// copying from c1.username to c.username
			strcpy(c.password, c1.password);
			int j;
			for(j=0; j<10;j++){
				c.booking_id[j] = c1.booking_id[j];
				if (c1.booking_id[j]!=0)
					deleteBooking(c1.booking_id[j]); //delete corresponding booking details
			}
			c.type = c1.type;
			c.status = 0;
			
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*1048)+1;
			lock.l_len = 1048;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 1048), SEEK_SET);
				
			write(fd, &c, 1048);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
			close(fd);
			return 1;
		}
		if(strncmp(c1.username, username, strlen(c1.username))==0 && c1.type==type && c1.status==0)
		{
			close(fd);
			return(2);
		}
		i++;
	}
	
	close(fd);
	return 0;
}


int deleteTrain(int train_num)
{
	int fd=open("train.txt", O_RDWR, 0766);
	int i=1;
	
	struct train t1, t;
	
	while(read(fd, &t1, sizeof(t1))>0)
	{		
		if(train_num - t1.train_num==0 && t1.status==1)
		{
			
			t.train_num = t1.train_num;
			t.total_seats = t1.total_seats+t1.booked_seats;
			t.booked_seats = 0;
			t.status = 0;
			deleteBookingBasedTrain(train_num); //delete all  bookings in the train	
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*16)+1;
			lock.l_len = 16;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 16), SEEK_SET);
				
			write(fd, &t, 16);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
			close(fd);
			return 1;
		}
		if(train_num - t1.train_num==0 && t1.status==0){
			close(fd);
			return 2;
		}
		i++;
	}
	
	close(fd);
	return 0;
}

 /************************UPDATE Function*************************/
 
 void updateTrainSeat(int train_num, int seat)
{
	int fd=open("train.txt", O_RDWR, 0766);
	int i=1;
	
	struct train t1, t;
	
	while(read(fd, &t1, sizeof(t1))>0)
	{		
		if(train_num - t1.train_num==0)
		{
			
			t.train_num = t1.train_num;
			t.total_seats = t1.total_seats + seat;
			t.booked_seats = t1.booked_seats;
			t.status = 1;
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*16)+1;
			lock.l_len = 16;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 16), SEEK_SET);
				
			write(fd, &t, 16);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);

		}
		i++;
	}
	
	close(fd);
}

int updateBooking(int booking_id, int seats, char *username){
	int fd=open("booking.txt", O_RDWR, 0766);
	int i=1;
	
	struct booking b1, b;
	bool outerflag=false;
	while(read(fd, &b1, sizeof(b1))>0)
	{		
		if((booking_id - b1.booking_id==0) && strcmp(b1.username, username)==0)
		{
			outerflag=true;
			if (b1.status==0)
				return 0; //update not possible for cancelled bookings
					
			b.booking_id = b1.booking_id;
			b.train_num = b1.train_num;
			strcpy(b.username, b1.username);
			b.status = 1;
			if(seats<=(-1*b1.seats_booked)){
				b.status=0; //making ticket cancel
				seats = -1*b1.seats_booked; //in this case, all tickets will get cancelled
			}
			b.seats_booked = b1.seats_booked+seats;
			
			/****Train num to be checked and seats to be uppdated****/
			int fd2 = open("train.txt", O_RDWR, 0766); //for Train
			struct train t1, t;
			int j=1;

			int flag=false;
			while(read(fd2, &t1, sizeof(t))>0)
			{
				if((t1.train_num - b1.train_num==0) && (t1.total_seats-t1.booked_seats>=seats))
				{
					flag = true;		
					t.train_num = t1.train_num;
					t.total_seats = t1.total_seats;
					t.booked_seats = t1.booked_seats + seats;
					if(t.booked_seats<0)
						t.booked_seats=0;
					t.status = t1.status;
						
					
					struct flock lock;
					lock.l_type = F_WRLCK;
					lock.l_whence = SEEK_SET;
					lock.l_start = ((j-1)*16)+1;
					lock.l_len = 16;
					lock.l_pid = getpid();
				
					fcntl(fd2, F_SETLKW, &lock);
						
					lseek(fd2, ((j-1) * 16), SEEK_SET);
						
					write(fd2, &t, 16);
						
					lock.l_type = F_UNLCK;
					fcntl(fd2, F_SETLK, &lock);
					break;

				}
				j++;
			}
			
			if(!flag)
				return 0; //required no. of seats not available
			/*******Train checked and seats checked **************/
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*516)+1;
			lock.l_len = 516;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 516), SEEK_SET);
				
			write(fd, &b, 516);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
			

		}
		i++;
	}
	
	close(fd);
	if (!outerflag)
		return 0;
	return 1;
}


void updateCustomerStatus(char *username, int choice, int status)
{
	int fd=open("customer.txt", O_RDWR, 0766);
	int i=1;
	
	struct customer c1, c;
	
	while(read(fd, &c1, sizeof(c1))>0)
	{		
		if(strncmp(c1.username, username, strlen(c1.username))==0 && c1.type==choice)
		{
			
			strcpy(c.username, c1.username);		// copying from c1.username to c.username
			strcpy(c.password, c1.password);
			int j;
			for(j=0; j<10;j++){
				c.booking_id[j] = c1.booking_id[j];
			}
			c.type = c1.type;
			c.status = status;
			
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*sizeof(struct customer))+1;
			lock.l_len = sizeof(struct customer);
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * sizeof(struct customer)), SEEK_SET);
				
			write(fd, &c, sizeof(struct customer));
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);

		}
		i++;
	}
	
	close(fd);
}


int resumeTrain(int train_num)
{
	int fd=open("train.txt", O_RDWR, 0766);
	int i=1;
	
	struct train t1, t;
	
	while(read(fd, &t1, sizeof(t1))>0)
	{		
		if(train_num - t1.train_num==0 && t1.status==0)
		{
			t.train_num = t1.train_num;
			t.total_seats = t1.total_seats;
			t.booked_seats = 0;
			t.status = 1;
			
			struct flock lock;
			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = ((i-1)*16)+1;
			lock.l_len = 16;
			lock.l_pid = getpid();
		
			fcntl(fd, F_SETLKW, &lock);
				
			lseek(fd, ((i-1) * 16), SEEK_SET);
				
			write(fd, &t, 16);
				
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
			return 1;
		}
		if(train_num - t1.train_num==0 && t1.status==1)
			return 2;
		i++;
	}
	
	close(fd);
	return 0;
}