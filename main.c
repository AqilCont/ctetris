/**
 * TODO:
 * - [2dgraphics] Set up scene API, make it as seamless as possible
 */

// Timing logic i can maybe look into: https://stackoverflow.com/questions/20390028/c-using-glfwgettime-for-a-fixed-time-step

// GLEW, for opengl functions
#define GLEW_STATIC
#include <GL/glew.h>

// Window management
#include <GLFW/glfw3.h>

#include "g/2dgraphics.h"
#include "tetris.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


GLFWwindow* window;

gamestate g = { .width = 640, .height = 480, .frames = 0 };

bool pressed = false;
pvec pressplace;
struct { double x, y; bool pressed; } mouse;


double frameaccum = 0;
char framerate[12];

int main (void) {
  
  /* Initialize the library */
  if (!glfwInit())
    return -1;
  
  // GLFW hints
  glfwWindowHint(GLFW_DECORATED, GL_FALSE);
  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(g.width, g.height, "Hello World", NULL, NULL);
  if (!window) {
    puts("couldn't initialize window???");
    glfwTerminate(); return -1; }
    
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
  // Set the framerate to the framerate of the screen / 2.
  // glfwSwapInterval(.5);
  
  // Initialize GLEW so we can reference OpenGL functions.
  if(glewInit()/* != GLEW_OK */) {
    printf("error with initializing glew");
    glfwTerminate(); return -1; }

  // Initializes text shaders and context
  glinitgraphics();
  typeface* font = loadfont("d:/projects/c/ctetris/res/fonts/iosevka-bold.ttf");
  doneloadingfonts();

  // Sets the coordinate system to screen 1:1 pixel mapping
  coords_screen();

  // GLFW input callbacks
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetKeyCallback(window, key_callback);

  // Makes a tetris board
  tetrisstate t = {.width = 10, .height = 20};
  tetinit(&t);
  t.x = 100;
  t.y = 50;

  imagedata* img = loadimage("D:\\projects\\c\\ctetris\\res\\img\\img1.png");
  imagedata* csm = loadimage("D:\\projects\\c\\ctetris\\res\\img\\csm.jpg");
  imagedata* bg = loadimage("D:\\projects\\c\\ctetris\\res\\img\\girl.jpg");

  bool textchanged = true;
  while(!glfwWindowShouldClose(window)) {

    // Framerate stuff
    g.delta = glfwGetTime() - g.last;
    g.last = glfwGetTime();
    g.frames ++;
    frameaccum += 1.0/g.delta;

    // If there have been more than 100 accumulated frames, reset the accumulation and update framecount
    if(g.frames % 100 == 0) {
      snprintf(framerate, sizeof(framerate), "%.2f fps", frameaccum/100);
      frameaccum = 0;
      textchanged = true;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    //setu2f("mouse", (vec2) { (float) mouse.x, (float) mouse.y });

    // fill(1.0f, .5f, .3f, 1.0f);
    // rect(g.width - 150, g.height - 50, 200, -20);
    
    if(textchanged) {
      fill(.5f, .5f, .5f, 1.0f);
      tsiz(20);
      text(framerate, g.width - 130, g.height - 10);
      textchanged = false;
    } else skiprec(strlen(framerate));

    tetdraw(&t);

    fill(1.0f, 1.0f, 1.0f, 1.0f);
    rect(0, 0, g.width, 1);
    rect(0, g.height-1, g.width, 1);
    rect(0, 0, 1, g.height);
    rect(g.width-1, 0, 1, g.height);

    fill(0.f, 0.f, 0.f, 0.f);
    image(csm, 0, 0, g.width, g.height);

    draw();
    //glfwSetWindowShouldClose(window, 1);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (action == GLFW_PRESS)
    if(button == GLFW_MOUSE_BUTTON_RIGHT)
      glfwSetWindowShouldClose(window, 1);
    else { pressed = true; glfwGetCursorPos(window, &pressplace.x, &pressplace.y); }
  else pressed = false;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
  mouse.x = xpos;
  mouse.y = ypos;
  if(pressed) {
    int wxpos, wypos;
    glfwGetWindowPos(window, &wxpos, &wypos);
    glfwSetWindowPos(window, (int) (xpos - pressplace.x) + wxpos, (int) (ypos - pressplace.y) + wypos);
  }
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  
}
