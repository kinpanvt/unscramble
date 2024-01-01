#include "bill.h"


// Reads date information from the file
void read_date(FILE *fp, Date *date) {
    fscanf(fp, "%4d%2d%2d", &date->year, &date->month, &date->day);
}

// Reads phone number information from the file
void read_phone(FILE *fp, PhoneNumber *phone) {
    fscanf(fp, "%3d%3d%4d", &phone->area_code, &phone->exchange_code, &phone->line);
}

// Reads a single call record from the file
void read_record(FILE *fp, CallRecord *record) {
    read_date(fp, &record->date);
    fscanf(fp, "%4d", &record->time);
    read_phone(fp, &record->phone_number);
    float temp_duration;
    fscanf(fp, "%f%f", &temp_duration, &record->cost);
    record->duration = (int)(temp_duration * 100);  // Convert float to integer (keeping two decimal places)
}

// Initialize a bill struct
void init_bill(Bill *bill) {
    bill->name = NULL;
    bill->address = NULL;
    bill->address2 = NULL;
    bill->records = malloc(sizeof(CallRecord) * 10);  // This line should work now
    bill->num_records = 0;
    bill->array_size = 10;
}


// Reads the entire bill from a file
void read_bill(FILE *fp, Bill *bill) {
    init_bill(bill);
    
    // Reading name and address (Stripping newline characters)
    size_t len = 0;
    getline(&bill->name, &len, fp);
    getline(&bill->address, &len, fp);
    getline(&bill->address2, &len, fp);  // Additional line for the address
    bill->name[strcspn(bill->name, "\n")] = 0;
    bill->address[strcspn(bill->address, "\n")] = 0;
    bill->address2[strcspn(bill->address2, "\n")] = 0;  // Strip newline for the additional address
    
    // Skip the remaining header lines (now 2 lines because we read an additional line for address)
    char buffer[256];
    for(int i = 0; i < 2; ++i) {
        fgets(buffer, sizeof(buffer), fp);
    }

    CallRecord record;
    int itemsRead;
    while (1) { // Loop until EOF is reached
        itemsRead = fscanf(fp, "%4d%2d%2d\t%4d\t%3d%3d%4d\t%d\t%f\n", 
                           &record.date.year, &record.date.month, &record.date.day,
                           &record.time,
                           &record.phone_number.area_code, &record.phone_number.exchange_code, &record.phone_number.line,
                           &record.duration, &record.cost);
        
        if (itemsRead == EOF) {
            break; // Stop the loop if end-of-file is reached
        }
        
        if (itemsRead != 9) {
            // fscanf couldn't read 9 items, something's wrong.
            continue; // Skip this loop iteration
        }

        add_record(bill, record);  // Fix for Issue 1: Make sure all records are read
    }
}

const char* convert_month_by_number(int month) {
    static const char* month_names[] = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
    };
    if (month >= 1 && month <= 12) {
        return month_names[month - 1];
    }
    return "Unknown";
}


void summarize(FILE *fp, Bill *bill, int month) {
    int total_calls = 0;
    int total_duration = 0;
    double total_cost = 0.0;
    
    for (int i = 0; i < bill->num_records; i++) {
        if (bill->records[i].date.month == month) {
            total_calls++;
            total_duration += bill->records[i].duration;
            total_cost += bill->records[i].cost;
        }
    }

    double avg_duration = total_calls ? (double)total_duration / total_calls : 0;
    double avg_cost = total_calls ? ((int)((total_cost * 100) / total_calls + 0.5)) / 100.0 : 0;


    avg_cost = ((int)(avg_cost * 100000000 + 0.5)) / 100000000.0;


    if (total_calls == 0) {
        fprintf(fp, "\t%12s:\tTotal %d\tAverage Duration: 0\t\tAverage Cost: 0\n",
                convert_month_by_number(month), total_calls);
    } else {
        fprintf(fp, "\t%12s:\tTotal %d\tAverage Duration: %.4f\tAverage Cost: %.2f\n",
                convert_month_by_number(month), total_calls, avg_duration, avg_cost);
    }
}




// Prints date to a file
void print_date(FILE *fp, Date date) {
    fprintf(fp, "%04d%02d%02d", date.year, date.month, date.day);
}

// Prints phone number to a file
void print_phone(FILE *fp, PhoneNumber phone) {
    fprintf(fp, "%03d%03d%04d", phone.area_code, phone.exchange_code, phone.line);
}

// Prints a single call record to a file
void print_record(FILE *fp, CallRecord record) {
    print_date(fp, record.date);
    fprintf(fp, "    %04d    ", record.time);
    print_phone(fp, record.phone_number);
    fprintf(fp, "  %d  %.2f\n", record.duration, record.cost);
}

// Adds a new record in sorted order
void add_record(Bill *bill, CallRecord new_record) {
    // Resize the array if needed
    if (bill->num_records == bill->array_size) {
        bill->array_size *= 2;
        bill->records = realloc(bill->records, sizeof(CallRecord) * bill->array_size);
    }
    // Find the position to insert the new record
    int pos = 0;
    while (pos < bill->num_records && 
           (bill->records[pos].date.year < new_record.date.year ||
           (bill->records[pos].date.year == new_record.date.year &&
            bill->records[pos].date.month < new_record.date.month) ||
           (bill->records[pos].date.year == new_record.date.year &&
            bill->records[pos].date.month == new_record.date.month &&
            bill->records[pos].date.day < new_record.date.day) ||
           (bill->records[pos].date.year == new_record.date.year &&
            bill->records[pos].date.month == new_record.date.month &&
            bill->records[pos].date.day == new_record.date.day &&
            bill->records[pos].time < new_record.time))) {
        pos++;
    }
    // Make a hole and insert the new record
    memmove(&bill->records[pos + 1], &bill->records[pos], (bill->num_records - pos) * sizeof(CallRecord));
    bill->records[pos] = new_record;
    bill->num_records++;
}


// Convert month integer to string
char* convert_month(int month) {
    char *months[] = {"", "January", "February", "March", "April", "May", "June",
                      "July", "August", "September", "October", "November", "December"};
    return months[month];
}

// Convert month string to integer
int convert_month_by_name(const char *month_name) {
    char *months[] = {"", "January", "February", "March", "April", "May", "June",
                      "July", "August", "September", "October", "November", "December"};
    for (int i = 1; i <= 12; i++) {
        if (strcmp(months[i], month_name) == 0) {
            return i;
        }
    }
    return 0;  // Not found
}



// Summarizes the entire bill
void summarize_bill(FILE *fp, Bill *bill) {
    for (int month = 1; month <= 12; month++) {
        summarize(fp, bill, month);
    }
}





// Clears any dynamic memory in call records
void clear_records(Bill *bill) {
    if (bill->records) {
        free(bill->records);
        bill->records = NULL;
    }
}

// Clears any dynamic memory in the bill
void clear_bill(Bill *bill) {
    if (bill->name) {
        free(bill->name);
        bill->name = NULL;
    }
    if (bill->address) {
        free(bill->address);
        bill->address = NULL;
    }
    clear_records(bill);
}


// Calculate average duration of all calls in the bill, rounded to 4 decimal places
float average_duration(Bill *bill) {
    int total_duration = 0;
    for (int i = 0; i < bill->num_records; i++) {
        total_duration += bill->records[i].duration;
    }
    float avg_duration = bill->num_records ? (float)total_duration / bill->num_records : 0;
    avg_duration = ((int)(avg_duration * 10000 + 0.5)) / 10000.0;  // Rounding to 4 decimal places
    return avg_duration;
    
}


// Calculate average cost of all calls in the bill, rounded to 2 decimal places
float average_cost(Bill *bill) {
    float total_cost = 0.0;
    for (int i = 0; i < bill->num_records; i++) {
        total_cost += bill->records[i].cost;
    }
    float avg_cost = bill->num_records ? total_cost / bill->num_records : 0;
    avg_cost = ((int)(avg_cost * 100 + 0.5)) / 100.0;  // Rounding to 2 decimal places
    return avg_cost;
}
