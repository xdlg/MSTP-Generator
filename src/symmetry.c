/**************************************************************************//**
 * @file
 * Point symmetry around the origin of an image.
 * Symmetrical points are found every 2pi/s, s being the symmetry order.
 * The goal is to build a linked list (symmetry elements) of linked lists
 * (coordinates). Each of the coordinates linked list represents a series of
 * symmetrical points. The list is (in theory) not redundant, i.e. a point
 * doesn't appear twice in a coordinates list. In practice, the calculations
 * are not perfect (floating point, etc.) so some points are part of two 
 * symmetries and some points are not included in any symmetry.
 *****************************************************************************/

/******************************************************************************
 * #include
 *****************************************************************************/
#include "symmetry.h"

/******************************************************************************
 * Private variables
 *****************************************************************************/
	
/******************************************************************************
 * Private functions
 *****************************************************************************/
static void add_sym(struct sym **head, struct sym **tail, struct xy *head_xy);
static void add_xy(struct xy **head, struct xy **tail, uint32_t x, uint32_t y);
static void free_xy(struct xy *head);
static void rotate(uint32_t dim, uint32_t x, uint32_t y, float a, int32_t *xr, 
	int32_t *yr);
 
/**************************************************************************//**
 * Build the list of lists of symmetrical coordinates.
 * @param head_sym pointer² to the first symmetry of the list (overwritten)
 * @param tail_sym pointer² to the last symmetry of the list (overwritten)
 * @param dim dimension of the image (assuming it's square)
 * @param s symmetry order
 *****************************************************************************/
void build_sym_list(struct sym **head_sym, struct sym **tail_sym, uint32_t dim,
	uint32_t s)
{
	struct xy *head_xy, *tail_xy;
	float a, slope;
	int32_t x, y;
	int32_t xt, yt;
	int32_t xr, yr;
	int32_t r = dim/2;
	uint32_t i;
	uint32_t leq_geq;
	
	// Initialize symmetry list
	*head_sym = NULL;
    *tail_sym = NULL;
	
	// 0 isn't accepted as a symmetry order
	if (s == 0)
	{
		printf("Symmetry order can't be 0\n");
	}
	
	// 1 means no symmetry, i.e. each point is individually included in the
	// symmetry list
	else if (s == 1)
	{
		for (x = 0; x < dim; x++)
		{
			for (y = 0; y < dim; y++)
			{
				head_xy = NULL;
				tail_xy = NULL;
				add_xy(&head_xy, &tail_xy, x, y);
				add_sym(head_sym, tail_sym, head_xy);
			}
		}
	}
	
	// Symmetry order >= 2
	else
	{
		// Compute the limit line
		slope = tanf(2.0 * (float)M_PI / (float)s);
		
		// For all coordinates in the top half of the image...
		for (x = 0; x < dim; x++)
		{
			for (y = 0; y < r; y++)
			{
				// Initialize coordinates list
				head_xy = NULL;
				tail_xy = NULL;
				
				// Translate coordinates
				xt = x - r;
				yt = r - y;
				
				// For symmetry orders under 5, points must be over the limit
				// line to be included in the list. For orders 5 and beyond,
				// under the limit line.
				leq_geq = (s < 5) ? GEQ : LEQ;

				if (compare((float)yt, (slope * (float)xt), leq_geq))
				{
					// Add the first coordinates to the list (i.e. the ones in
					// the main zone)
					add_xy(&head_xy, &tail_xy, x, y);
					
					// Add the symmetrical points to the list, only if they're
					// within the image
					for (i = 1; i < s; i++)
					{
						a = 2.0 * (float)i * (float)M_PI / (float)s;
						rotate(dim, x, y, a, &xr, &yr);
						if ((xr < dim) && (xr >= 0) && (yr < dim) && (yr >= 0))
						{
							add_xy(&head_xy, &tail_xy, xr, yr);
						}
					}
					
					// Add one symmetry to the list, including the coordinates
					// list that has just been built
					add_sym(head_sym, tail_sym, head_xy);
				}
			}
		}
	}	
}

/**************************************************************************//**
 * Add one symmetry at the end of the list.
 * For the first element of the list, this function must be called with head = 
 * NULL and tail = NULL.
 * @param head pointer² to the first symmetry of the list (overwritten)
 * @param tail pointer² to the last symmetry of the list (overwritten)
 * @param head_xy pointer to the first element of the xy list
 *****************************************************************************/
void add_sym(struct sym **head, struct sym **tail, struct xy *head_xy)
{
    struct sym *previous = *tail;
    *tail = malloc(sizeof(struct sym));
    (*tail)->head_xy = head_xy;
    (*tail)->next = NULL;
    if (*head == NULL)
	{
		*head = *tail; // First call
	}
	else
	{	
		previous->next = *tail;
	}
}

/**************************************************************************//**
 * Add one xy couple at the end of the list.
 * For the first element of the list, this function must be called with head = 
 * NULL and tail = NULL.
 * @param head pointer² to the first xy of the list (overwritten)
 * @param tail pointer² to the last xy of the list (overwritten)
 * @param x x coordinate
 * @param y y coordinate
 *****************************************************************************/
void add_xy(struct xy **head, struct xy **tail, uint32_t x, uint32_t y)
{
    struct xy *previous = *tail;
    *tail = malloc(sizeof(struct xy));
    (*tail)->x = x;
    (*tail)->y = y;
    (*tail)->next = NULL;
    if (*head == NULL)
	{
		*head = *tail; // First call
	}
	else
	{	
		previous->next = *tail;
	}
}

/**************************************************************************//**
 * Free a list of symmetries and all xy coordinates.
 * @param head pointer to the first symmetry of the list
 *****************************************************************************/
void free_sym(struct sym *head)
{
    struct sym *current = head;
    struct sym *next;
    
    while (current != NULL)
    {
		free_xy(current->head_xy);
		next = current->next;
		free(current);
		current = next;
    }
}

/**************************************************************************//**
 * Free a list of xy coordinates.
 * @param head pointer to the first xy of the list
 *****************************************************************************/
void free_xy(struct xy *head)
{
    struct xy *current = head;
    struct xy *next;
    
    while (current != NULL)
    {
		next = current->next;
		free(current);
		current = next;
    }
}

/**************************************************************************//**
 * Rotate point coordinates around the center of an image.
 * This function does not check if the rotated point is still within the
 * image's bounds. The caller must take care of that if needed.
 * @param dim image dimension
 * @param x original x coordinate
 * @param y original y coordinate
 * @param a rotation angle (rad)
 * @param xr rotated x coordinate (overwritten)
 * @param yr rotated y coordinate (overwritten)
 *****************************************************************************/
void rotate(uint32_t dim, uint32_t x, uint32_t y, float a, int32_t *xr, 
	int32_t *yr)
{
	float cos_a, sin_a;
	int32_t xt, yt;
	int32_t r = dim/2;
	
	// xt and yt are the translated coordinates so that the center of the image
	// is at (0, 0). Then the rotated coordinates are translated back to the
	// original system.
	cos_a = cosf(a);
	sin_a = sinf(a);
	xt = x - r;
	yt = r - y;
	*xr = (uint32_t)((float)xt * cos_a - (float)yt * sin_a + (float)r);
	*yr = (uint32_t)((float)r - (float)yt * cos_a - (float)xt * sin_a);
}

/**************************************************************************//**
 * Check the symmetry list graphically.
 * Generate an array of 0s to represent the image. Change all points in the 
 * symmetry list to 1, 2, ..., n depending on their symmetry level. Points that
 * are not part of any symmetry stay at 0.
 * @param head_sym pointer to the first symmetry of the list
 * @param dim image dimension
 *****************************************************************************/
void check_sym_list(struct sym *head_sym, uint32_t dim)
{
    struct sym *current_sym = head_sym;
    struct xy *current_xy;
    uint32_t x, y, i;
    uint32_t im[dim][dim];
    
    for (x = 0; x < dim; x++)
	{
		for (y = 0; y < dim; y++)
		{
			im[x][y] = 0;
		}
	}
    
    while (current_sym != NULL)
    {
		current_xy = current_sym->head_xy;
		i = 0;
		while (current_xy != NULL)
		{
			im[current_xy->x][current_xy->y] = ++i;
			current_xy = current_xy->next;
		}
        current_sym = current_sym->next;
    }
    
    for (y = 0; y < dim; y++)
	{
		for (x = 0; x < dim; x++)
		{
			printf("%d ", im[x][y]);
		}
		printf("\n");
	}

}
 
/**************************************************************************//**
 * Apply a symmetry rule to the image.
 * @param head_sym pointer to the first symmetry of the list
 * @param dim image dimension
 * @param im image (overwritten)
 *****************************************************************************/
void symmetrize(struct sym *head_sym, uint32_t dim, float im[][dim])
{
	struct sym *current_sym = head_sym;
    struct xy *current_xy;
    float avg;
    uint32_t i;
	
	while (current_sym != NULL)
    {
		// First pass through the coordinates list: compute average
		avg = 0.0;
		i = 0;
		current_xy = current_sym->head_xy;
		while (current_xy != NULL)
		{
			avg += im[current_xy->x][current_xy->y];
			i++;
			current_xy = current_xy->next;
		}
		
		// Second pass: replace point values by average
		avg = avg / (float)i;
		current_xy = current_sym->head_xy;
		while (current_xy != NULL)
		{
			im[current_xy->x][current_xy->y] = avg;
			current_xy = current_xy->next;
		}
		
        current_sym = current_sym->next;
    }
}
