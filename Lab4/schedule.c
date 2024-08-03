#include "types.h"
#include "user.h"

void help()
{
    printf(1, "Commands List:\n");
    printf(1, "  info\n");
    printf(1, "  change_queue <pid> <new_queue>\n");
    printf(1, "  set_bjf_proc <pid> <priority_ratio> <arrival_time_ratio> <executed_cycle_ratio> <process_size_ratio>\n");
    printf(1, "  set_bfj_sys <priority_ratio> <arrival_time_ratio> <executed_cycle_ratio> <process_size_ratio>\n");
}

void info()
{
    print_process_info();
}

void set_queue(int pid, int new_queue)
{
    if (pid < 1)
    {
        printf(1, "Invalid pid\n");
        return;
    }
    if (new_queue < 1 || new_queue > 3)
    {
        printf(1, "Invalid queue\n");
        return;
    }
    int res = change_scheduling_queue(pid, new_queue);
    if (res < 0)
        printf(1, "Error changing queue\n");
    else
        printf(1, "Queue changed successfully\n");
}

void set_process_bjf(int pid, int priority_ratio, int arrival_time_ratio, int executed_cycle_ratio, int process_size_ratio)
{
    if (pid < 1)
    {
        printf(1, "Invalid pid\n");
        return;
    }
    if (priority_ratio < 0 || arrival_time_ratio < 0 || executed_cycle_ratio < 0)
    {
        printf(1, "Invalid ratios\n");
        return;
    }
    int res = set_bjf_params_proc(pid, priority_ratio, arrival_time_ratio, executed_cycle_ratio, process_size_ratio);
    if (res < 0)
        printf(1, "Error setting BJF params\n");
    else
        printf(1, "BJF params set successfully\n");
}

void set_system_bjf(int priority_ratio, int arrival_time_ratio, int executed_cycle_ratio, int process_size_ratio)
{
    if (priority_ratio < 0 || arrival_time_ratio < 0 || executed_cycle_ratio < 0)
    {
        printf(1, "Invalid ratios\n");
        return;
    }
    set_bjf_params_sys(priority_ratio, arrival_time_ratio, executed_cycle_ratio, process_size_ratio);
    printf(1, "BJF params set successfully\n");
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        help();
        exit();
    }
    if (!strcmp(argv[1], "info"))
        info();
    else if (!strcmp(argv[1], "change_queue"))
    {
        if (argc < 4)
        {
            help();
            exit();
        }
        set_queue(atoi(argv[2]), atoi(argv[3]));
    }
    else if (!strcmp(argv[1], "set_bjf_proc"))
    {
        if (argc < 6)
        {
            help();
            exit();
        }
        set_process_bjf(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
    }
    else if (!strcmp(argv[1], "set_bjf_sys"))
    {
        if (argc < 5)
        {
            help();
            exit();
        }
        set_system_bjf(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    }
    else
        help();
    exit();
}
