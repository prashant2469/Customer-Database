#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define SIZE 20
#define MAX_NAME 256

typedef struct Customer {
	char *email;
	char *name;
	char *shoesize;
	char *food;
	struct Customer *next;
}Customer;

typedef struct CustomerTable{
	Customer ** buckets;
	size_t num_buckets;
}CustomerTable;

unsigned long hash(char *str) {
	unsigned long hash = 5381;
	int c;

	while (*str != '\0') {
		c = *str;
		hash = ((hash << 5) + hash) + (unsigned char)c; /* hash * 33 + c */
		str++;
	}
	return hash;
	}
Customer *add_customer_to_list(char *email, char *name, char *shoesize, char *food, Customer *bucket) {
	Customer* new_customer;
	new_customer = calloc(1, sizeof(Customer));
	new_customer->email = strdup(email);
	new_customer->name = strdup(name);
	new_customer->food = strdup(food);
	new_customer->shoesize = strdup(shoesize);
	new_customer->next = bucket;

	return new_customer;
}
Customer *change(char *name, char *shoesize, char *food, Customer *change){
	change->name = strdup(name);
	change->food = strdup(food);
	change->shoesize = strdup(shoesize);
	return change;
}
void add_customer_to_table(char *email, char *name, char *shoesize, char *food, CustomerTable *table) {
	unsigned long hashvalue = hash(email);
	size_t which_bucket = hashvalue % table->num_buckets;
	Customer *linkedlist = table->buckets[which_bucket];
	table->buckets[which_bucket] = add_customer_to_list(email, name, shoesize, food, linkedlist);
}

Customer *lookup(char *email, CustomerTable *table) {
	unsigned long hashvalue = hash(email);
	unsigned long which_bucket = hashvalue % table->num_buckets;
	Customer *here = table->buckets[which_bucket];
	while(here){
		if(!strcmp(here->email, email)){
			printf("Customer found\n");
			printf("Email = %s\n", here->email);
			printf("Name = %s\n", here->name);
			printf("Shoe Size = %s\n", here->shoesize);
			printf("Favorite Food = %s\n", here->food);
			printf("\n \n");
			return here;
		}
		here = here->next;
	}
	printf("friend not found");
	return NULL;
}
Customer *delete_friend_from_list(char *email, Customer *linkedlist){
	if (!linkedlist) {
    return NULL;
  }else if((!strcmp(email, linkedlist->email))){
	   Customer *next = linkedlist->next;
	   free(linkedlist->email);
	   free(linkedlist->food);
	   free(linkedlist->name);
	   free(linkedlist->shoesize);
	   free(linkedlist);
	   return next;
  }
	return NULL;
}
bool delete(char *email, CustomerTable *table) {
	unsigned long hashvalue = hash(email);
  	size_t which_bucket = hashvalue % table->num_buckets;
	Customer *linkedlist = table->buckets[which_bucket];
	bool found = false;
  	Customer *here = linkedlist;
	while(here) {
		if (!strcmp(here->email, email)) {
			found = true;
			break;
		}
		here = here->next;
	}
	if (!found) {
    return false;
  } 
	table->buckets[which_bucket] = delete_friend_from_list(email, linkedlist);
  return true;

}
void list(CustomerTable *list) {
	for(size_t i = 0; i<list->num_buckets; i++) {
		if(list->buckets){
			Customer *here = list->buckets[i];
			while(here){
				printf("\n \n");
				printf("Email = %s\n",here->email);
				printf("Name = %s\n", here->name);
				printf("Shoe Size = %s\n", here->shoesize);
				printf("Favorite Food = %s\n", here->food);
				here = here->next;
      		}

    	}	
		else{
			continue;
		}
		
	}

}
void save(CustomerTable *save){
	const char* STUDENT_DATA_FORMAT = "%s\t%s\t%s\t%s\n";
	FILE *fp = fopen("customers.tsv", "w");

	for(size_t i = 0; i<save->num_buckets; i++) {
		if(save->buckets){
		struct Customer *here = save->buckets[i];
		while(here){
			fprintf(fp,STUDENT_DATA_FORMAT,here->email,here->name,here->shoesize,here->food);
			here = here->next;
		}

		}
		else{
			continue;
	}
	
	}
	fclose(fp);
}

CustomerTable *build_friend_table(size_t num_buckets) {
  CustomerTable* output = calloc(1, sizeof(CustomerTable));
  output->num_buckets = num_buckets;

  // allocate space for num_buckets FriendNode pointers.
  // THEY ARE ALREADY NULL POINTERS.
  output->buckets = calloc(num_buckets, sizeof(CustomerTable *));
  return output;
}
void delete_friend_table(CustomerTable *table) {
  // for each bucket, delete everything in that bucket
  for (size_t i=0; i < table->num_buckets; i++) {
    Customer *here = table->buckets[i];
    while(here) {
      free(here->email);
	  free(here->name);
      free(here->food);
	  free(here->shoesize);
      Customer *freethis = here;
      here = here->next;
      free(freethis);
    }
  }
  free(table->buckets);
  free(table);
}



int main(void){
	char strcmd[50];
	bool stop = true;

	CustomerTable* ptr = build_friend_table(50);

	int index = 0;
	char line[50];
	FILE *fp = fopen("customers.tsv", "r");
	if(fp == NULL){
		printf("Could not open the file\n");
	}
	while(fgets(line, 80 ,fp)){
		char *email2;
		char *name2;
		char *shoesize2;
		char *favfood2;

		char* token = strtok(line,"\t");
		index = 0;
	   	while(token!=NULL){
			if(index == 0){
				email2 = strdup(token);
			}
			if(index == 1){
				name2 = strdup(token);
			}
			if(index == 2){
				shoesize2 = strdup(token);
			}
			if(index == 3){
				favfood2 = strdup(token);
				int n = strcspn(favfood2, "\n");
				favfood2[n] = 0;
			}
			index++;
			token = strtok(NULL, "\t");
		}
		add_customer_to_table(email2,name2,shoesize2,favfood2, ptr);
		free(email2);
		email2 = NULL;
		free(name2);
		name2 = NULL;
		free(shoesize2);
		shoesize2 = NULL;
		free(favfood2);
		favfood2 = NULL;
		
	}
	list(ptr);
	while(stop){
		char email2[50];
		char name2[50];
		char shoesize2[50];
		char favfood2[50];
		printf("command: ");
		scanf("%s", strcmd);
		if(strcmp(strcmd, "quit") == 0){
			delete_friend_table(ptr);
			free(line);
			break;
		}
		else if(strcmp(strcmd, "save") == 0){
			save(ptr);
		}
		else if(strcmp(strcmd, "add") == 0){
			
			while(getchar() != '\n');
			printf("email address?: \n");

			scanf("%s", email2);
			while(getchar() != '\n');
			printf("Name?: \n");

			scanf("%[^\n]", name2);
			while(getchar() != '\n');
			printf("shoe size?: \n");

			scanf("%s", shoesize2);

			while(getchar() != '\n');
			printf("favorite food?: \n");
			scanf("%[^\n]", favfood2);
			while(getchar() != '\n');

			int hashvalue = hash(email2);
			int which_bucket = hashvalue % ptr->num_buckets;


			Customer *ll = ptr->buckets[which_bucket];
			bool value = false;
			while(ll){
				if(strcmp(ll->email, email2) == 0){
					ll = change(name2, shoesize2, favfood2, ll);
					value = true;
					break;
				}
			}
			

			if(!ll){
				add_customer_to_table(email2, name2, shoesize2, favfood2, ptr);
			}

			list(ptr);
		}
		else if(strcmp(strcmd, "lookup") == 0){
			printf("Enter email:");
			scanf("%s", email2);
			lookup(email2, ptr);
		}
		else if(strcmp(strcmd, "delete") == 0){
			printf("Enter email to delete:");
			scanf("%s", email2);
			if(lookup(email2, ptr) == NULL){
				printf("Could not find customer to delete.\n");
			}
			else{
				delete(email2, ptr);
				printf("Customer with email %s is deleted successfully.\n",email2);
			}
		}
		else if(strcmp(strcmd, "list") == 0){
			list(ptr);
		}
		else{
			printf("unkown command \n");
		}

	}
fclose(fp);
}

