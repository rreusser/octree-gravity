#ifndef __OCTREE_DRAW_H__
#define __OCTREE_DRAW_H__

// SAME AS octree_gravity.c, BUT DRAW AS WE GO...
point octree_descend_node_forces_draw( ot_node* target, ot_node* otn, particles* p );
point octree_add_forces_draw( octree* ot, particles* p, int i );
point octree_add_node_forces_draw( ot_node* target, ot_node* otn, ot_node* child, particles* p );

#endif /*__OCTREE_DRAW_H__*/
