#ifndef __OCTREE_GRAVITY_H__
#define __OCTREE_GRAVITY_H__


typedef struct ot_struct {
    struct ot_struct **children;
    struct ot_struct *parent;
    real dx;
    unsigned short int hasPt;
    point* pt;
} ot_node;

typedef struct {
    int n;
    ot_node** index;
    point center;
    real dx;
    ot_node* root;
    particles* p;
} octree;

// Create a new top-level octree structure
octree* alloc_octree(int n);

// Free the octree data structure
void free_octree( octree* ot );


// Create the individual nodes
ot_node* alloc_ot_node();

// Free a node
void free_ot_node( ot_node* otn );

// Debug functions
void traverse_debug(octree* ot);
void traverse_debug_ot_node( ot_node* otn );

// MAIN gravity function to be called from integrator
void octree_gravity( real t, real* y, int n, real* yp, void* data );

// Draw the tree structure
void draw_children( ot_node* otn );

// Draw the cubes:
void draw_octree_node( ot_node* otn, real dx, point center, int num );
void draw_octree( octree* ot );

// Search until we find where to insert a node (NOT ROBUST FOR DUPLICATE POINTS!!!)
void insert_point_mass_node( ot_node** otn, ot_node** parent, point center, point* insert, real dx );
void insert_point_mass( octree* ot, real x, real y, real z, real m, int ind );

// Locate the bounds - put a *cube* around the outermost points, not a retangular solid
void get_bounds( octree* ot, particles* p );

// Recursively calculate the center of mass
point ot_node_calculate_masses( ot_node* otn );
void octree_calculate_masses( octree* ot );

// Self-explanatory
void build_octree_from_particles( octree* ot, particles* p );

// Calculate and store in advance which leaves the particles fall under,
// so we don't have to search every time we need to figure this out.  Not
// really necessary considering the time it takes to calculate forces!
void build_index( octree* ot, particles* p );
void debug_index( octree* ot, particles* p );

// Locate which node corresponds to a certain point
ot_node* locate_ot_node( ot_node* otn, real x, real y, real z, real dx, point center );

// Descend the tree until the rest of the particles are insiginificant
// enough that we can just calculate a force from the center of mass
point octree_descend_node_forces( ot_node* target, ot_node* otn, particles* p );

// Add up the forces acting on a single particle i
point octree_add_forces( octree* ot, particles* p, int i );

// Ascend the tree until we reach the top, after which we can descend
// and add the forces up - need to do it this way so a force does not act on itself
point octree_add_node_forces( ot_node* target, ot_node* otn, ot_node* child, particles* p );

#endif /*__OCTREE_GRAVITY_H__*/
