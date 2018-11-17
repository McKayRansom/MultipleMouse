struct mouse{
  int x;
  int y;
};

//updates mouse position based on change in position from
//the global pointer (allMouse)
void mouse_update(struct mouse* myMouse, struct mouse* allMouse, XDeviceMotionEvent event) {
  int newX = event.x_root;
  int newY = event.y_root;
  int dx = newX - (allMouse->x);
  int dy = newY - (allMouse->y);
  myMouse->x += dx;
  myMouse->y += dy;
  allMouse->x = newX;
  allMouse->y = newY;
  printf("%ld mouse move : [%d, %d, %d, %d]\n",event.deviceid, dx, dy, myMouse->x, myMouse->y);
}
