#ifndef __GL_MOUSE_H__
#define __GL_MOUSE_H__

#define VIEW_ROTATE 0
#define VIEW_ZOOM 1
#define VIEW_TRANSLATE 2

void glMouseFunc(int button, int state, int x, int y);
void glMotionFunc(int x, int y);
void mouse(int button, int state, int x, int y);
void mouse2(int x, int y);

#endif /*__GL_MOUSE_H__*/
