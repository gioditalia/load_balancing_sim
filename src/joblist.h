struct Job *new_Job(int nCpu, int startTime, int jobTime);
struct Job *add_Job(struct Job *list ,struct Job *ele);
struct Job *assign_Job(struct Job *list, int CPU);
struct Job extract_Job(struct Job **list);
struct Job extract_Head_Job(struct Job **list);

int check_list(struct Job *list);

void print_Jobs(struct Job *list);

