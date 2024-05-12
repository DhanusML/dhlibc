#include "../include/dhlibc.h"

t_list *
dh_lstnew(void *content)
{
    t_list *new_list = (t_list *)malloc(sizeof(t_list));
    new_list -> content = content;
    new_list -> next = 0;
    return new_list;
}


void
dh_lstadd_front(t_list **lst, t_list *new)
{
    if (!new) return;
    t_list *buffer = *lst;
    *lst = new;
    while(new->next) {new = new->next;}
    new->next = buffer;
}


int
dh_lstsize(t_list *lst)
{
    size_t size = 0;
    while (lst)
    {
        size++;
        lst = lst->next;
    }
    return size;
}


t_list *
dh_lstlast(t_list *lst)
{
    while (lst->next) 
    {
        lst = lst->next;
    }
    return lst;
}


void
dh_lstadd_back(t_list **lst, t_list *new)
{
    t_list *old_list = *lst;
    while(old_list->next)
    {
        old_list = old_list->next;
    }
    old_list->next = new;
}


void
dh_lstdelone(t_list *lst, void (*del)(void*))
{
    del(lst->content);
    lst->next = 0;
    free(lst);
}


void
dh_lstclear(t_list **lst, void (*del)(void *))
{
    t_list  *curr_node = *lst;
    t_list *temp_node = 0;
    while (curr_node)
    {
        temp_node = curr_node;
        curr_node = curr_node->next;
        dh_lstdelone(temp_node, del);
    }
    *lst = 0;
}


void
dh_lstiter(t_list *lst, void (*f)(void *))
{
    while(lst)
    {
        f(lst->content);
        lst = lst->next;
    }
}


t_list *
dh_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
    t_list *new_lst = 0;
    t_list *new_tail = 0;
    t_list *temp_node = 0;
    temp_node = dh_lstnew(f(lst->content));
    new_lst = temp_node;
    new_tail = temp_node;
    del(lst->content);
    lst = lst->next;
    while (lst)
    {
        temp_node = dh_lstnew(f(lst->content));
        new_tail->next = temp_node;
        new_tail = temp_node;
        del(lst->content);
        lst = lst->next;
    }
    return new_lst;
}
