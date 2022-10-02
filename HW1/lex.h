typedef struct Record* RecordPointer;
typedef struct Record{
    int index;
    char* data;
    RecordPointer next;
}Record;

typedef struct TableHead{
    int tableSize;
    RecordPointer first;
}TableHead;

int isID_or_LETTER(char c){
    if((c>='a' && c<='z') || (c>='A' && c<='Z'))
        return 1;
    else
        return 0;
}

int isDIGIT(char c){
    if((c>='0' && c<='9'))
        return 1;
    else
        return 0;
}

int isSTRING(char c){
    if((c=='\"') || (c=='\''))
        return 1;
    else
        return 0;
}

int isOPERATOR(char c){
    if(c=='+')  return 1;
    else if(c=='-') return 2;
    else if(c=='*') return 3;
    else if(c=='/') return 4;
    else if(c=='=') return 5;
    else if(c==':') return 6;
    else return 0;
}

int isBLANK(char c){
    if(c==' ' || c=='\0' || c=='\n')
        return 1;
    else
        return 0;
}

int isESCAPE_SEQ(char c){
    if( c=='a'||c=='b'||c=='e'||c=='f'||c=='n'
        ||c=='r'||c=='t'||c=='v'||c=='\\'||c=='\''||c=='?')
        return 1;
    else
        return 0;
}

void init_table();

int get_id(char* line, int start, int line_num);
int get_string(char* line, int start, int line_num);
int get_other(char* line, int start, int line_num);
void analyze_line(char* line, int line_num);

int add_data_table(TableHead* head, char* data);
void print_table(TableHead* head);
void print_all();
void free_table(TableHead* head);
void free_all();