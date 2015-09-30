#include <assert.h>

#include "common.h"

/**
  These are the different types of Cells.
*/
typedef enum
{
    NUMBER,
    WORD,
    KEYWORD,
    NUL
} Type;

/**
  All of Orava's built-in keywords.
 */
typedef enum
{
    _then,
    _else,
    _end,
    _iter,
    _endit,
    _call,
    _quote,
    _endq,
    _eval,
    q_keywords
} Keywords;

char* keywords[] = {"then", "else",  "end",
		    "iter", "endit", "call",
		    "[",    "]",     "eval",
		     0};

typedef struct
{
    Type type;
    int  v;
} Cell;

typedef struct
{
    Cell cells[LIST_SIZE];
    int  size;
    int  index;
} List;

typedef struct
{
    char name[NAME_SIZE];
    List v;
} Word;

typedef struct
{
    Word words[TABLE_SIZE];
    int  size;
} Table;

Table table;
List  stack_a;
List  stack_b;

/**
   Returns a new cell of a specific type and value
*/
static Cell create_cell(Type type, int v)
{
    Cell r_cell={.type=type, .v=v};
    return r_cell;
}

/**
   Returns whether two cells are equal (sharing both type and value)
*/
static bool cmp_cell(Cell* cell, Type type, int v)
{
    return (cell->type==type && cell->v==v);
}

/**
   Returns a new cell which is a copy of the one exact supplied.
*/
static Cell copy_cell(Cell* cell)
{
    Cell r_cell={.type=cell->type, .v=cell->v};
    return r_cell;
}

/**
   Returns a list that can hold cells.
*/
static List create_list()
{
    List r_lst={.size=0, .index=0};
    return r_lst;
}

/**
   Pushes a cell to the tail of a list.
*/
static void push_cell(List* list, Cell* cell)
{
    if (list->size>=LIST_SIZE) {
	output("Error: Size of a list/word/stack too large.\n");
    } else {
    list->cells[list->size] = copy_cell(cell);
    list->size++;
    }
}

/**
   Returns a "null cell" a cell without type nor value.
*/
static Cell null_cell()
{
    Cell r_cell={.v=0, .type=NUL};
    return r_cell;
}

/**
   Removes and returns the head of a list.
*/
Cell pop_cell(List* list)
{
    if (list->size==0) {
	output("Error: underflow.\n");
	return null_cell();
    }
    Cell r_cell = copy_cell(&list->cells[list->size-1]);
    list->size--;
    return r_cell;
}

/**
   Creates an exact copy of a list.
*/
static List copy_list(List* list)
{
    List r_lst = create_list();
    int i;
    for (i=0; i<list->size; i++) {
	push_cell(&r_lst, &list->cells[i]);
    }
    return r_lst;
}

/**
   Returns a new cell of type "WORD" with a specific value(name).
*/
static Word create_word(char name[], List* v)
{
    Word r_word;
    _strcpy(r_word.name, name);
    r_word.v = copy_list(v);
    return r_word;
}

/**
   Returns an exact copy of a "WORD".
*/
static Word copy_word(Word* word)
{
    Word r_word;
    _strcpy(r_word.name, word->name);
    r_word.v = copy_list(&word->v);
    return r_word;
}

/**
   Returns a simple lookup table.
*/
static Table create_table()
{
    Table r_table={.size=0};
    return r_table;
}

/**
   Searches the lookup table for a specific string.
*/
static int search_table(Table* table, char name[])
{
    int i;
    for (i=0; i<table->size; i++) {
	if (_streql(table->words[i].name, name)) {
	    return i;
	}
    }
    return -1;
}

/**
   Pushes a "WORD" onto the table.
*/
static int push_word(Table* table, Word* word)
{
    int p = search_table(table, word->name);
    if (p<0) {
	table->words[table->size] = copy_word(word);
	return table->size++;
    }
    table->words[p] = copy_word(word);
    if (word->name[0] != '_') {
	// TODO: warn redefinition?
    }
    return table->size;
}

static int search_keyword(char name[])
{
    int i=0;
    while (keywords[i]!=0) {
	if (_streql(name, keywords[i])) {
		return i;
	}
	i++;
    }
    return -1;
}

static Cell str_to_cell(Table* table, char str[])
{
    Cell cell;
    /* Number? */
    if (is_int(str)) {
	cell=create_cell(NUMBER, to_int(str));
	return cell;
    }
    /* Keyword? */
    int p=search_keyword(str);
    if (p!=-1) {
	cell=create_cell(KEYWORD, p);
	return cell;
    }
    /* Word? */
    p=search_table(table, str);
    if (p!=-1) {
	cell=create_cell(WORD, p+q_keywords);
	return cell;
    } else {
	output("Symbol not found.\n");
	return null_cell();
    }
}

static Word str_to_word(Table* table, char str[])
{
    char strs[LIST_SIZE][NAME_SIZE];
    int  size = split(strs, str);
    List r_lst = create_list();
    
    if (size <= 2) {
	output("Warning: Empty definition.\n");
    }		

    int i;
    for (i=2; i<size; i++) {
	Cell cell = str_to_cell(table, strs[i]);
	push_cell(&r_lst, &cell);
    }

    Word r_word = create_word(strs[1], &r_lst);
    return r_word;
}

/* Compile a valid Orava program as a string */
static void compile(char str[])
{
    Word word = str_to_word(&table, str);
    push_word(&table, &word);
}

static void print()
{
    int a = pop_cell(&stack_a).v;
    char tmp[32];
    to_str(tmp, a);
    output(tmp);
}

static void add()
{
    int b = pop_cell(&stack_a).v;
    int a = pop_cell(&stack_a).v;
    Cell cell = create_cell(NUMBER, a+b);
    push_cell(&stack_a, &cell);
}

static void multiply()
{
    int b = pop_cell(&stack_a).v;
    int a = pop_cell(&stack_a).v;
    Cell cell = create_cell(NUMBER, a*b);
    push_cell(&stack_a, &cell);
}

static void divide()
{
    int b = pop_cell(&stack_a).v;
    int a = pop_cell(&stack_a).v;
    Cell cell = create_cell(NUMBER, a/b);
    push_cell(&stack_a, &cell);
}

static void mod()
{
    int b = pop_cell(&stack_a).v;
    int a = pop_cell(&stack_a).v;
    Cell cell = create_cell(NUMBER, a%b);
    push_cell(&stack_a, &cell);
}

static void negate()
{
    int a = pop_cell(&stack_a).v;
    Cell cell = create_cell(NUMBER, -a);
    push_cell(&stack_a, &cell);
}

static void n_copy()
{
    Cell a = pop_cell(&stack_a);
    /*
    if (stack_a.size<a.v) {
	output("Error: underflow while making copy!\n");
	return;
    }
    */
    int i;
    for (i=0; i<a.v; i++) {
	Cell t = stack_a.cells[stack_a.size-a.v-i];
	push_cell(&stack_a, &t);
    }
}

static void n_drop()
{
    int i;
    Cell a = pop_cell(&stack_a);
    for (i=0; i<a.v; i++) {
	pop_cell(&stack_a);
    }
}

static void equal()
{
    Cell b = pop_cell(&stack_a);
    Cell a = pop_cell(&stack_a);
    Cell cell = create_cell(NUMBER, a.v==b.v);
    push_cell(&stack_a, &cell);
}

static void not()
{
    Cell a = pop_cell(&stack_a);
    Cell cell = create_cell(NUMBER, !a.v);
    push_cell(&stack_a, &cell);
}

static void negative()
{
    Cell a = pop_cell(&stack_a);
    Cell cell = create_cell(NUMBER, a.v<0);
    push_cell(&stack_a, &cell);
}

static void or()
{
    Cell b = pop_cell(&stack_a);
    Cell a = pop_cell(&stack_a);
    Cell cell = create_cell(NUMBER, a.v||b.v);
    push_cell(&stack_a, &cell);
}

static void and()
{
    Cell b = pop_cell(&stack_a);
    Cell a = pop_cell(&stack_a);
    Cell cell = create_cell(NUMBER, a.v&&b.v);
    push_cell(&stack_a, &cell);
}

static void n_swap()
{
    Cell a = pop_cell(&stack_a);
    // Disabled errors on Arduino until I find a cross device
    // way of notifying them.
    /*
    if (&stack_a->size<a.v) {
	output("Error: underflow while swapping!\n");
	return;
    }
    */
    Cell t = stack_a.cells[stack_a.size-a.v-1];
    stack_a.cells[stack_a.size-a.v-1] = stack_a.cells[stack_a.size-1];
    stack_a.cells[stack_a.size-1] = t;
}

static void n_push()
{
    Cell a = pop_cell(&stack_a);
    // Disabled errors on Arduino until I find a cross device
    // way of notifying them.
    /*
    if (stack_a.size<a.v) {
	output("Error: underflow while popping!\n");
	return;
    }
    */
    int i;
    for (i=0; i<a.v; i++) {
	Cell cell = pop_cell(&stack_a);
	push_cell(&stack_b, &cell);
    }
}

static void n_pop()
{
    Cell a = pop_cell(&stack_a);
    // Disabled errors on Arduino until I find a cross device
    // way of notifying them.
    /*
    if (&stack_b->size<a.v) {
	output("Error: underflow while popping!\n");
	return;
    }
    */
    int i;
    for (i=0; i<a.v; i++) {
	Cell cell = pop_cell(&stack_b);
	push_cell(&stack_a, &cell);
    }
}

static void clear()
{
    stack_a.size=0;
}

Cell f_pop()
{
    Cell a = pop_cell(&stack_a).v;
    return a;
}

/**
   Replaces the entire stack with a list containing the previous stack
*/
static List list_from_stack(List* stack_a)
{
    List stack_t = create_list();
    List r_lst=create_list(0);
    int l = pop_cell(stack_a).v;
    int i;
    /* Two for loops in order to reverse list */
    for (i=0; i<l; i++) {
	Cell t_cell=pop_cell(stack_a);
	push_cell(&stack_t, &t_cell);
    }
    for (i=0; i<l; i++) {
	Cell t_cell=pop_cell(&stack_t);
	push_cell(&r_lst, &t_cell);
    }
    return r_lst;
}

static void eval_pos(Table*, List*, List*, int);

/**
   Evaluates a list of instructions
*/
static void eval_lst(Table* table, List* stack_a, List* stack_b, List* list)
{
    int i,j;
    int q_if=0,oq_if,o_if=1;
    int q_it=0,oq_it;
    int c, p;
    List it_word; List t_lst;
    Cell t_cell;
    void(*func)(Table*,List*,List*);
    Cell a;
    for (i=0; i<list->size; i++) {
	Cell cell = list->cells[i];
	switch (cell.type) {
	case NUMBER:
	    push_cell(stack_a, &cell);
	    break;
	case KEYWORD:
	    assert(cell.v<q_keywords);
	    switch (cell.v) {
	    case _call:
		func=(void(*)(Table*,List*,List*))pop_cell(stack_a).v;
		func(table, stack_a, stack_b);
		break;
	    case _then:
		a = pop_cell(stack_a);
		o_if  = a.v;
		oq_if = q_if;
		if (a.v!=0) break;
		for(;;) {
		    if (i>=LIST_SIZE) {
			printf("Error: if without endif\n");
			break;
		    }
		    if (cmp_cell(&list->cells[i], KEYWORD, _else)) {
			if (q_if==oq_if) break;
		    } else if (cmp_cell(&list->cells[i], KEYWORD, _end)) {
			if (q_if>oq_if) q_if--; else if (q_if==oq_if) break;
		    }
		    i++;
		    if (cmp_cell(&list->cells[i], KEYWORD, _then)) q_if++;
		}
		break;
	    case _else:
		oq_if = q_if;
		if (o_if==0) break;
		for(;;) {
		    if (i>=LIST_SIZE) {
			printf("Error: if without endif\n");
			break;
		    }
		    if (cmp_cell(&list->cells[i], KEYWORD, _end)) {
			if (q_if==oq_if) break;
		    }
		    i++;
		    if (cmp_cell(&list->cells[i], KEYWORD, _then)) q_if++;
		}
		break;
	    case _end:
		break;
	    case _iter:
		a = pop_cell(stack_a);
		q_it=0;
		it_word=create_list();
		for(;;) {
		    if (i>=LIST_SIZE) {
			printf("Error: iter without endit\n");
			break;
		    }
		    oq_it=q_it;
		    if (cmp_cell(&list->cells[i], KEYWORD, _endit)) {
			if (q_it==oq_it) break;
		    }
		    i++;
		    push_cell(&it_word, &list->cells[i]);
		    if (cmp_cell(&list->cells[i], KEYWORD, _iter)) q_it++;
		}
		pop_cell(&it_word);
		for (j=0; j<a.v; j++) {
		    eval_lst(table, stack_a, stack_b, &it_word);
		}
		break;
		/*
	    case _while:
		q_it=0;
		it_word=create_list();
		for(;;) {
		    if (i>=LIST_SIZE) {
			printf("Error: unbalanced parentheses.\n");
			break;
		    }
		    oq_it=q_it;
		    if (cmp_cell(&list->cells[i], KEYWORD, _endwhile)) {
			if (q_it==oq_it) break;
		    }
		    i++;
		    push_cell(&it_word, &list->cells[i]);
		    if (cmp_cell(&list->cells[i], KEYWORD, _while)) q_it++;
		}
		pop_cell(&it_word);
		eval_lst(table, stack_a, stack_b, &it_word);
		while (pop_cell(stack_a).v) {
		    eval_lst(table, stack_a, stack_b, &it_word);
		}
		break;
	    case _endwhile:
		break;
		*/
	    case _quote:
		c=0; p=0;
		t_lst=create_list();
		for(j=0; j<LIST_SIZE; j++) {
		    /* _quote */
		    if (cmp_cell(&list->cells[i], KEYWORD, _quote)) {
			p++;
			t_cell=create_cell(NUMBER, c+1);
			push_cell(&t_lst, &t_cell);

			c=0;
		    }
		    /* _endq */
		    else if (cmp_cell(&list->cells[i], KEYWORD, _endq)) {
			p--;
			t_cell=create_cell(NUMBER, c);
			push_cell(stack_a, &t_cell);

			c=pop_cell(&t_lst).v+c;
		    }
		    /* number/word */
		    else {
			push_cell(stack_a, &list->cells[i]);
			c++;
		    }
		    if (p==0) break;
		    i++;
		}
		break;
	    case _endq:
		break;
	    case _eval:
		t_lst = list_from_stack(stack_a);
		eval_lst(table, stack_a, stack_b, &t_lst);
		break;
	    case _endit:
		break;
	    default:
		assert(0);
	    }
	    break;
	case WORD:
	    assert(cell.v-q_keywords<table->size);
	    eval_pos(table, stack_a, stack_b, cell.v-q_keywords);
	    break;
	default:
	    printf("Warning: Evaluating null-cell");
	}
    }
}

static void eval_pos(Table* table, List* stack_a, List* stack_b, int table_pos)
{
    Word word  = table->words[table_pos];
    List list = word.v;
    eval_lst(table, stack_a, stack_b, &list);
}

static void eval_str(Table* table, List* stack_a, List* stack_b, char str[])
{
    if (str[0] == ':') {
	compile(str);
	return;
    }
    char tmp[STRING_SIZE] = ": __eval__ ";
    int i=0;
    while (str[i]!='\0') {
	tmp[11]=str[i];
	i++;
    }
    tmp[i]='\0';
    compile(tmp);
    eval_pos(table, stack_a, stack_b, search_table(table, "__eval__"));
}

/**
   Prepare built-in commands.
*/
void compile_builtin(char str[], void(*func)())
{
    char t_str[64] = ": ";
    char t_str2[16];
    char t_str3[] = " call";
    to_str(t_str2, (int)func);

    int i=0,l=2;
    while(str[i]!='\0') {
	t_str[i+l]=str[i];
	i++;
    }
    t_str[i+l]=' '; i++;
    l=i+l; i=0;
    while(t_str2[i]!='\0') {
	t_str[i+l]=t_str2[i];
	i++;
    }
    l=i+l; i=0;
    while(t_str3[i]!='\0') {
	t_str[i+l]=t_str3[i]; i++;
    }
    t_str[i+l]='\0';

    compile(t_str);
}

/**
   Prepare Orava by compiling all built-in functions.
*/
void init()
{
    table   = create_table();
    stack_a = create_list();
    stack_b = create_list();

    compile_builtin("print",  &print);
    compile_builtin("+",      &add);
    compile_builtin("*",      &multiply);
    compile_builtin("/",      &divide);
    compile_builtin("%%",     &mod);
    compile_builtin("negate", &negate);
    compile_builtin("equal?", &equal);
    compile_builtin("not",    &not);
    compile_builtin("-?",     &negate);
    compile_builtin("and",    &and);
    compile_builtin("or",     &or);
    compile_builtin("n_swap", &n_swap);
    compile_builtin("n_push", &n_push);
    compile_builtin("n_pop" , &n_pop);
    compile_builtin("n_drop", &n_drop);
    compile_builtin("clear",  &clear);
    compile_builtin("n_copy", &n_copy);
}

/**
 Evaluates a valid Orava string.
 */
void eval(char str[])
{
    if ((str[0]=='/'&&str[1]=='/')
	||str[0]=='\0'
	||str[0]=='\n') return;
    int i=0,c=0;
    while (str[i]!='\0'){
	if (!iswhite(str[i])) c++;
	i++;
    }
    if (c < 2) {
	output("Error: invalid command.\n");
	return;
    }
    eval_str(&table, &stack_a, &stack_b, str);
}
