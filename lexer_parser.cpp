#include <stdio.h>

#define STR_MAX 256

/**
 * lexer
 * =================
 */

struct lexer_tr{ // lexer transitions
    char src;
    int state;
} sample_tran[] = {
    // // text_book_version
    // /*00*/ {'+',5},
    // /*01*/ {'*',6},
    // /*02*/ {'(',7},
    // /*03*/ {')',8},
    // /*04*/ {' ',4},
    // /*05*/ {'-',1},
    // /*06*/ {'0',2},
    // /*07*/ {'1',2},
    // /*08*/ {'2',2},
    // /*09*/ {'3',2},
    // /*10*/ {'4',2},
    // /*11*/ {'5',2},
    // /*12*/ {'6',2},
    // /*13*/ {'7',2},
    // /*14*/ {'8',2},
    // /*15*/ {'9',2},
    // /*16*/ {'.',3}

    // quiz_version
    {'=',1},
    {'+',2},
    {'-',3},
    {'*',4},
    {'/',5},
    {';',6},
    {'(',7},
    {')',8},
    {' ',9},
    {'0',10}, // 10
    {'1',10},
    {'2',10},
    {'3',10},
    {'4',10},
    {'5',10},
    {'6',10},
    {'7',10},
    {'8',10},
    {'9',10},
    {'.',11},
    {'E',12},
    {'0',13}, // 11,13
    {'1',13},
    {'2',13},
    {'3',13},
    {'4',13},
    {'5',13},
    {'6',13},
    {'7',13},
    {'8',13},
    {'9',13}, // 11^
    {'E',12}, // 13^
    {'0',14}, // 12,14
    {'1',14},
    {'2',14},
    {'3',14},
    {'4',14},
    {'5',14},
    {'6',14},
    {'7',14},
    {'8',14},
    {'9',14}, // 14^
    {'+',14},
    {'-',14}, // 12^
    {'>',18},
    {'}',19},
    {'{',20},
    {'i',21},
    {'f',22} // 21

};

struct lexer_st{ // lexer states
    int rule_start;
    int rule_end;
    char token; // 0 as reject, 'D' as a digits, ' ' as a blank which will be ignored
    int spacial_move; // int spacial_move(char* cursor,char* token); // return next state
} sample_sta[] = {
    // // text_book_version
    // /*00*/ {0,15,0},
    // /*01*/ {6,15,0},
    // /*02*/ {6,16,'D'},
    // /*03*/ {6,15,0},
    // /*04*/ {4,4,' '},
    // /*05*/ {0,-1,'+'},
    // /*06*/ {0,-1,'*'},
    // /*07*/ {0,-1,'('},
    // /*08*/ {0,-1,')'}

    // quiz_version
    {0,0,0,1}, // 0
    {0,-1,'=',0},
    {0,-1,'+',0},
    {0,-1,'-',0},
    {0,-1,'*',0},
    {0,-1,'/',0}, // 5
    {0,-1,';',0},
    {0,-1,'(',0},
    {0,-1,')',0},
    {0,-1,' ',0},
    {9,20,'n',0}, // 10
    {21,30,0,0},
    {32,43,0,0},
    {21,31,'n',0},
    {32,41,'n',0},
    {0,0,0,3}, // 15
    {0,-1,'d',0},
    {0,-1,'i',2},

    {0,-1,'>',0},
    {0,-1,'}',0},
    {0,-1,'{',0}, // 20
    {48,48,0,0},
    {0,-1,'f',0},

};

/**
 * special scan
 * =================
 */

int spacial_move(int type,char** cursor,char* token){
    switch(type){
    case 1:
        *token = 0;
        if(**cursor >= '0' && **cursor <= '9'){
            (*cursor)++;
            return 10;
        }
        if(**cursor >= 'A' && **cursor <= 'Z'){
            (*cursor)++;
            return 17;
        }
        if(**cursor == 'd'){
            (*cursor)++;
            return 15;
        }
        for (int i = 0; i <= 8; ++i)
        {
            if(sample_tran[i].src == **cursor){
                (*cursor)++;
                // printf("[%d]\n",sample_tran[i].state);
                return sample_tran[i].state;
            }
        }
        for (int i = 44; i <= 47; ++i)
        {
            if(sample_tran[i].src == **cursor){
                (*cursor)++;
                // printf("[%d]\n",sample_tran[i].state);
                return sample_tran[i].state;
            }
        }
        break;
    case 2:
        if(**cursor >= '0' && **cursor <= '9'){
            (*cursor)++;
            return 17;
        }
        if(**cursor >= 'A' && **cursor <= 'Z'){
            (*cursor)++;
            return 17;
        }
        *token = 'i';
        break;
    case 3:
        char* eclare = "eclare";
        *token = 0;
        for (int i = 0; i < 6; ++i)
        {
            if(**cursor != *(eclare+i)){
                (*cursor)++;
                throw "Unknown token! Did you mean 'declare'?";
            }
            (*cursor)++;
        }
        return 16;
        break;
    }
    return -1;
}

class Lexer
{
public:
    Lexer(char* input,lexer_tr* rules,lexer_st* states){
        this->input = input;
        this->rules = rules;
        this->states = states;
        this->cursor = this->input;
    }

    char next_ptn(int status){
        // printf("<%c,%d> ",*(this->cursor),status);
        lexer_st* cur = this->states + status;
        char token = 0;
        int next_state = -1;
        // printf(">> %d > %s\n",status,this->cursor);
        if(cur->spacial_move){
            
            next_state = spacial_move(cur->spacial_move,&(this->cursor),&token);
        }
        else{
            for (int i = cur->rule_start; i <= cur->rule_end; ++i)
                if(*(this->cursor) == this->rules[i].src){
                    this->cursor++;
                    next_state = this->rules[i].state;
                }
            token = cur->token;
        }
        if(next_state >= 0)
            return this->next_ptn(next_state);
        if(!token)
            throw "Unknown token!";
        return token;
    }

    char* scan(){
        char* output = new char[STR_MAX];
        char* cursor = output;
        char ptn;
        while(*(this->cursor)){
            // printf("\n>> %s\n", this->cursor);
            ptn = this->next_ptn(0);
            // printf("\n{%c}\n",ptn );
            if(ptn != ' '){
                *cursor = ptn;
                cursor++;
            }
        }
        *cursor = 0;
        return output;
    }

private:
    int status;
    char* input;
    char* cursor;
    lexer_tr* rules;
    lexer_st* states;
};

/**
 * Parser (LL1 parser)
 */

struct parse_der{ // parsing derivations
    char src;
    char* der;
} sample_der[] = {
    // // text_book_version
    // /*00*/ {'D',"E\0"}, // S
    // /*01*/ {'(',"E\0"},
    // /*02*/ {'D',"TF"}, // E
    // /*03*/ {'(',"TF"},
    // /*04*/ {'+',"+E"}, // F
    // /*05*/ {')',""},
    // /*06*/ {'\0',""},
    // /*07*/ {'D',"DU"},  // T
    // /*08*/ {'(',"(E)"},
    // /*09*/ {'+',""}, // U
    // /*10*/ {'*',"*T"},
    // /*11*/ {')',""},
    // /*12*/ {'\0',""}

    {'f',"T\0"}, // 0
    {'d',"T\0"},
    {'f',"I"},
    {'d',"A"},
    {'d',"MB"},
    {'d',"A"}, // 5
    {'}',""},
    {'\0',""},
    {'f',"f(i>n){A}"},
    {'d',"di=E;"},
    {'n',"RG"}, // 10
    {'i',"RG"},
    {'(',"RG"},
    {'+',"+RG"},
    {'-',"-RG"},
    {';',""}, // 15
    {')',""},
    {'n',"FQ"},
    {'i',"FQ"},
    {'(',"FQ"},
    {'+',""}, // 20
    {'-',""},
    {'*',"*FQ"},
    {'/',"/FQ"},
    {';',""},
    {')',""}, // 25
    {'n',"n"},
    {'i',"i"},
    {'(',"(E)"}

    // quiz_version
};


struct parse_nter{ // parsing non-terminals
    char nter;
    int rule_start;
    int rule_end;
} sample_nter[] = {
    // // text_book_version
    // /*00*/ {'S',0,1},
    // /*01*/ {'E',2,3},
    // /*02*/ {'F',4,6},
    // /*03*/ {'T',7,8},
    // /*04*/ {'U',9,12}

    {'S',0,1},
    {'T',2,3},
    {'A',4,4},
    {'B',5,7},
    {'I',8,8},
    {'M',9,9},
    {'E',10,12},
    {'G',13,16},
    {'R',17,19},
    {'Q',20,25},
    {'F',26,28}

    // quiz_version
};

class Parser{
public:
    Parser(char* input,parse_der* rules,parse_nter* nter,int nter_n){
        this->input = input;
        this->rules = rules;
        this->nter = nter;
        this->cursor = this->input;
        this->nter_n = nter_n;
        this->level = 0;
    }

    void parse(){
        this->parse(0);
    }

    void parse(char nter){
        int i = this->nter2nter_i(nter);
        if(i == -1)
            throw "Unknown non-terminals!";
        this->parse(i);
    }

    void parse(int nter_i){
        parse_nter* nter = this->nter + nter_i;
        char* tar;
        for (int i = nter->rule_start; i <= nter->rule_end; ++i)
            if(*(this->cursor) == this->rules[i].src){
                tar = this->rules[i].der;
                goto syntax_found;
            }
        printf(">> %s\n",this->cursor);
        throw "Unknown Syntax!";
        syntax_found:
        this->print_level();
        printf("[%s] ... %s\n",tar,this->cursor);
        int nter_tmp;
        while(*tar){
            // printf("[%c] <> [%c] => ",*tar,*(this->cursor) );
            if(*tar == *(this->cursor)){
                tar++;
                this->cursor++;
            }
            else{
                // printf("!=\n");
                this->level++;
                parse(nter2nter_i(*tar));
                this->level--;
                this->print_level();
                printf("$ ... %s\n",this->cursor);
                // printf("~%c\n",*tar);
                tar++;
            }
        }
    }

private:
    char* input;
    char* cursor;
    parse_der* rules;
    parse_nter* nter;
    int nter_n;
    int level;

    void print_level(){
        for (int i = 0; i < this->level; ++i)
            printf("  ");
    }

    int nter2nter_i(char nter){
        for (int i = 0; i < this->nter_n; ++i)
            if(this->nter[i].nter == nter)
                return i;
        return -1;
    }
};

/**
 * main
 * =================
 */

int main(int argc, char const *argv[])
{
    char* input = new char[STR_MAX];

    Lexer* l;
    Parser* p;
    char* lexer_r;
    while(true){
        try{
            printf("Please input (press enter directly to exit)\n>> ");
            gets(input);
            if(!*input)
                break;
            printf("Your input: [%s]\n",input);
            l = new Lexer(input,sample_tran,sample_sta);
            lexer_r = l->scan();
            printf("Lexer Result: [%s]\n",lexer_r);
            printf("Press any key to start parsing...\n");
            getchar();
            p = new Parser(lexer_r,sample_der,sample_nter,sizeof(sample_nter)/sizeof(parse_nter));
            p->parse();
            printf("Parseing process successfully done!\n");
        }catch(char const* msg){
            printf("ERROR: %s\n",msg );
        }
    }
    printf("Exiting...\n");

    return 0;
}
