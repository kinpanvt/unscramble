#include "bill.h"

// Summarizes the entire bill including yearly summary
void summarize_bill_yearly(FILE *fp, Bill *bill, int year) {
    int yearly_calls = 0;
    int yearly_duration = 0;
    float yearly_cost = 0.0;
    
    for (int month = 1; month <= 12; month++) {
        summarize(fp, bill, month);
    }
    
    for (int i = 0; i < bill->num_records; i++) {
        if (bill->records[i].date.year == year) {
            yearly_calls++;
            yearly_duration += bill->records[i].duration;
            yearly_cost += bill->records[i].cost;
        }
    }
    
    float yearly_avg_duration = yearly_calls ? (float) yearly_duration / yearly_calls : 0;
    float yearly_avg_cost = yearly_calls ? yearly_cost / yearly_calls : 0;

    // Formatting for Year Summary
    fprintf(fp, "    Year %d:\tTotal %d\tAverage Duration: %.4f\tAverage Cost: %.6f\n",
            year, yearly_calls, yearly_avg_duration, yearly_avg_cost);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    FILE *input_fp = fopen(argv[1], "r");
    FILE *output_fp = fopen(argv[2], "w");
    
    if (!input_fp || !output_fp) {
        fprintf(stderr, "Error opening files.\n");
        return 1;
    }
    
    Bill bill;
    
    char line[256];
    while (fgets(line, sizeof(line), input_fp)) {
        if (line[0] == '#') continue;  // skip comment lines

        char command[50];
        sscanf(line, "%s", command);
        fprintf(output_fp, "Command: %s ", command);

        if (strcmp(command, "load") == 0) {
            char bill_file[50];
            sscanf(line + strlen(command), "%s", bill_file);
            fprintf(output_fp, "%s\n", bill_file);
            FILE *bill_fp = fopen(bill_file, "r");
            if (bill_fp) {
                read_bill(bill_fp, &bill);
                fprintf(output_fp, "\tLoaded %d records from %s\n", bill.num_records, bill_file);
                fclose(bill_fp);
            } else {
                fprintf(output_fp, "\tError loading %s\n", bill_file);
            }
        } else if (strcmp(command, "summarize") == 0) {
            char month[50];
            if (sscanf(line + strlen(command), "%s", month) == 1) {
                fprintf(output_fp, "%s\n", month);
                summarize(output_fp, &bill, convert_month_by_name(month));
            } else {
                fprintf(output_fp, "\n");
                summarize_bill_yearly(output_fp, &bill, 2020);  // Assuming the year is 2020
            }
        } else if (strcmp(command, "average") == 0) {
            char type[50];
            sscanf(line + strlen(command), "%s", type);
            fprintf(output_fp, "%s\n", type);
            if (strcmp(type, "duration") == 0) {
                fprintf(output_fp, "\tAverage Duration: %.4f\n", average_duration(&bill));
            } else if (strcmp(type, "cost") == 0) {
                fprintf(output_fp, "\tAverage Cost: %.2f\n", average_cost(&bill));
            }
        } else if (strcmp(command, "display") == 0) {
            fprintf(output_fp, "\n");
            
            fprintf(output_fp, "%s\n%s\n%s\n\n", bill.name, bill.address, bill.address2);
    
            // Formatting for Display
            fprintf(output_fp, "Date\t\tTime\tNumber\t\tDrtn\tCost\n");
            fprintf(output_fp, "-----------------------------------------------------\n");
            for (int i = 0; i < bill.num_records; i++) {
                print_record(output_fp, bill.records[i]);
            }
        }
    }
    
    fclose(input_fp);
    fclose(output_fp);
    clear_bill(&bill);
    
    return 0;
}
