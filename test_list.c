#include "./include/dhlibc.h"
#include <stdio.h>

void print_new(void *elt)
{
    printf("%s\n", (char *)elt);
}

void
print_list(t_list *list)
{
    while (list)
    {
        print_new(list->content);
        list = list->next;
    }
}

char dh_toupper_char(char x){return (char) dh_toupper((char)x);}

void *
toUpper(void *str)
{
    return dh_strmapi(str, dh_toupper_char);
}


int
main()
{
    t_list *names_list = dh_lstnew("m");
    t_list *new_name = dh_lstnew("dhanus");
    t_list *new_name2 = dh_lstnew("lal");
    //print_list (names_list);
    dh_lstiter(names_list, print_new);
    printf("%d\n", dh_lstsize(names_list));
    dh_lstadd_front(&names_list, new_name);
    dh_lstiter(names_list, print_new);
    dh_lstadd_back(&names_list, new_name2);
    dh_lstiter(names_list, print_new);
    dh_lstiter(dh_lstlast(names_list), print_new);
    printf("%d\n", dh_lstsize(names_list));
    //dh_lstclear(&names_list, print_new);
    //printf("%d\n", dh_lstsize(names_list));
    //dh_lstiter(names_list, print_new);
    char *caps =  toUpper("dhanus");
    printf("%s\n", caps);
    dh_lstiter(names_list, print_new);
    printf("********\n");
    t_list *upper_list = dh_lstmap(names_list, toUpper, print_new);
    printf("********\n");
    dh_lstiter(upper_list, print_new);
    return 0;
}
