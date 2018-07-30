#ifndef N_NODE_H
#define N_NODE_H

#if !defined(__cplusplus)
typedef int bool;
#define true 1
#define false 0
#endif

#include <malloc.h>
#ifndef n_alloc
#define n_alloc(size) malloc(size)
#endif

#include <assert.h>

typedef void* n_node;
typedef int node_type;

typedef struct
{
    node_type type; // uint?
    n_node next;
} n_node_header;

typedef struct n_list
{
    n_node first_node;
    unsigned int node_count; // Remove?
} n_list;

static n_list *new_list()
{
    n_list *result = (n_list *)n_alloc(sizeof(n_list));
    if(result)
    {
        result->first_node = 0;
        result->node_count = 0;
    }

    return result;
}

static n_list *new_list_node(n_node node)
{
    n_list *result = new_list();
    result->first_node = node;
    return result;
}

#define N_NODE_HEADER(ptr) ((n_node_header *)ptr - 1)

// TODO: Make iterator struct { int index; }
#define for_each_node(first, type) type it=0;int it_index=0;for(it=(type)(first);it;it=(type)get_next_node(it),++it_index)
#define get_it(data_type) ((data_type)(it))

inline node_type
get_node_type(n_node node)
{
    return N_NODE_HEADER(node)->type;
}

inline void
set_node_type(n_node node, node_type type)
{
    N_NODE_HEADER(node)->type = type;
}

inline n_node
get_next_node(n_node node)
{
    return N_NODE_HEADER(node)->next;
}

#define new_node(type) (type *)new_node_(sizeof(type))
#define new_node_type(node_type, type) (type *)new_node_type_(node_type, sizeof(type))
#define new_node_data(data_ptr, type) (type *)new_node_data_(((void *)data_ptr), sizeof(type))

static n_node
new_node_(size_t data_size) // node_type type
{
    n_node result;
    n_node_header *header;

    size_t header_size = sizeof(n_node_header);
    // size_t struct_size = sizeof(n_node_union);
    void *ptr = n_alloc(header_size + data_size);
    if(ptr == 0) return 0;

    memset((char *)ptr, 0, header_size + data_size);
    result = (n_node *)((char *)ptr + header_size);
    header = N_NODE_HEADER(result);
    // header->type = type;
    header->next = 0;

    return result;
}

static n_node
new_node_type_(node_type type, size_t data_size)
{
    n_node result = new_node_(data_size);
    set_node_type(result, type);
    return result;
}

static n_node
new_node_data_(void *data, size_t data_size) // node_type type
{
    n_node result = new_node_(data_size);
    memcpy(result, data, data_size);
    return result;
}

#define copy_node(node, type) (type *)copy_node_(node, sizeof(type))
static n_node
copy_node_(n_node node, size_t data_size)
{
    n_node result = new_node_data_(node, data_size);
    N_NODE_HEADER(result)->type = get_node_type(node);
    N_NODE_HEADER(result)->next = get_next_node(node);
    return result;
}

// TODO: Node removing

// TODO: get_node_at
inline n_node
get_node_index(n_list *list, int index_at)
{
    n_node node_at_index = 0;
    
    for_each_node(list->first_node, n_node)
    {
        if(index_at == it_index)
        {
            node_at_index = it;
            break;
        }
    }
    
    return node_at_index;
}

inline n_node
get_first_node(n_list *list)
{
    return list->first_node;
}

inline n_node
get_last_node(n_list *list)
{
    n_node result = 0;
    if(list)
    {
        n_node ptr = list->first_node;
        while(get_next_node(ptr))
        {
            ptr = get_next_node(ptr);
        }

        result = ptr;
    }

    return result;
}

inline void
insert_after(n_node node, n_node new_node)
{
    // TODO: node_count won't be incremented this way!
    assert(node);
    assert(new_node);
    N_NODE_HEADER(new_node)->next = get_next_node(node);
    N_NODE_HEADER(node)->next = new_node;
}

inline void
insert_front(n_list *list, n_node new_node)
{
    assert(new_node);
    N_NODE_HEADER(new_node)->next = list->first_node;
    list->first_node = new_node;
    ++list->node_count;
}

inline void
insert_back(n_list *list, n_node new_node)
{
    assert(list);
    if(new_node)
    {
        if(list->first_node)
        {
            n_node last_node = get_last_node(list);
            N_NODE_HEADER(last_node)->next = new_node;
        }
        else
        {
            list->first_node = new_node;
        }
		// TODO: do we need node counter?
        ++list->node_count;
    }
}

inline bool
is_list_empty(n_list list)
{
    bool result = false;
    if(!list.first_node) result = true;
    return result;
}

#endif
