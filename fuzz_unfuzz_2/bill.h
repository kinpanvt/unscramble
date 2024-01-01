#ifndef BILL_H
#define BILL_H
#define MAX_RECORDS 100 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int year;
    int month;
    int day;
} Date;

typedef struct {
    int area_code;
    int exchange_code;
    int line;
} PhoneNumber;

typedef struct {
    Date date;
    int time;
    PhoneNumber phone_number;
    int duration;
    float cost;
} CallRecord;

typedef struct {
    char *name;
    char *address;
    char *address2; 
    int num_records;
    int array_size;
    CallRecord *records;;
} Bill;



void read_date(FILE *fp, Date *date);
void read_phone(FILE *fp, PhoneNumber *phone);
void read_record(FILE *fp, CallRecord *record);
void init_bill(Bill *bill);
void read_bill(FILE *fp, Bill *bill);
void print_date(FILE *fp, Date date);
void print_phone(FILE *fp, PhoneNumber phone);
void print_record(FILE *fp, CallRecord record);
void add_record(Bill *bill, CallRecord new_record);
char* convert_month(int month);
int convert_month_by_name(const char *month_name);
void summarize(FILE *fp, Bill *bill, int month);
void summarize_bill(FILE *fp, Bill *bill);
float average_cost(Bill *bill);
float average_duration(Bill *bill);
void clear_records(Bill *bill);
void clear_bill(Bill *bill);

#endif // BILL_H
