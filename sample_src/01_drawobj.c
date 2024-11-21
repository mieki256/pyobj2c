// Last updated: <2024/05/02 03:08:54 +0900>
//
// Draw wavefront obj. use GLFW3
//
// by mieki256
// License: CC0 / Public Domain

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
// #include <SOIL/SOIL.h>

#include "cube01.h"
#include "suzanne01.h"
#include "car.h"
#include "scooter.h"

#define WDW_TITLE "Draw wavefront obj"

// Window size
#define SCRW 1280
#define SCRH 720

#define FOV 50.0

#define ENABLE_LIGHT 1

typedef struct modeldata
{
  float scale;
  const int vtx_size;
  const float *vtx;
  const int nml_size;
  const float *nml;
  const int col_size;
  const float *col;
  const int uv_size;
  const float *uv;
} MODELDATA;

#define MODELS_LEN 4

MODELDATA models[MODELS_LEN] = {
    {// cube01
     3.0,
     cube01_obj_vtx_size, &cube01_obj_vtx[0][0],
     cube01_obj_nml_size, &cube01_obj_nml[0][0],
     cube01_obj_col_size, &cube01_obj_col[0][0],
     cube01_obj_uv_size, &cube01_obj_uv[0][0]},
    {// suzanne01
     8.0,
     suzanne01_obj_vtx_size, &suzanne01_obj_vtx[0][0],
     suzanne01_obj_nml_size, &suzanne01_obj_nml[0][0],
     suzanne01_obj_col_size, &suzanne01_obj_col[0][0],
     suzanne01_obj_uv_size, &suzanne01_obj_uv[0][0]},
    {// car
     8.0,
     car_obj_vtx_size, &car_obj_vtx[0][0],
     car_obj_nml_size, &car_obj_nml[0][0],
     car_obj_col_size, &car_obj_col[0][0],
     car_obj_uv_size, &car_obj_uv[0][0]},
    {// scooter
     8.0,
     scooter_obj_vtx_size, &scooter_obj_vtx[0][0],
     scooter_obj_nml_size, &scooter_obj_nml[0][0],
     scooter_obj_col_size, &scooter_obj_col[0][0],
     scooter_obj_uv_size, &scooter_obj_uv[0][0]},
};

typedef struct
{
  int scrw;
  int scrh;
  float fovy;
  float znear;
  float zfar;

  double angle;
  double angle_x;
  int model_kind;
} GWK;

static GWK gw;

// ----------------------------------------
// Render
void render(void)
{
  gw.angle += (45.0 / 60.0);

  // init OpenGL
  glViewport(0, 0, gw.scrw, gw.scrh);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(gw.fovy, (double)gw.scrw / (double)gw.scrh, gw.znear, gw.zfar);
  glMatrixMode(GL_MODELVIEW);

  // clear screen
  glClearColor(0.25, 0.25, 0.25, 1);
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_NORMALIZE);

  glLoadIdentity();

#if ENABLE_LIGHT
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  const float light_pos[4] = {1.0, 1.0, 1.0, 0.0};
  const float light_ambient[4] = {0.2, 0.2, 0.2, 1.0};
  const float light_diffuse[4] = {0.8, 0.8, 0.8, 1.0};
  const float light_specular[4] = {0.7, 0.7, 0.7, 1.0};

  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
#endif

  // obj move and rotate
  float scale = models[gw.model_kind].scale;
  glTranslatef(0.0, 0.0, -20.0);
  glScalef(scale, scale, scale);
  glRotatef(gw.angle_x, 1, 0, 0);
  // glRotatef(gw.angle * 0.5, 1, 0, 0);
  glRotatef(gw.angle, 0, 1, 0);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  // glCullFace(GL_FRONT);
  glCullFace(GL_BACK);

  // draw vertex array
  glEnableClientState(GL_VERTEX_ARRAY);
  // glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, models[gw.model_kind].vtx);
  glTexCoordPointer(2, GL_FLOAT, 0, models[gw.model_kind].uv);
  glNormalPointer(GL_FLOAT, 0, models[gw.model_kind].nml);
  glColorPointer(4, GL_FLOAT, 0, models[gw.model_kind].col);

  glDrawArrays(GL_TRIANGLES, 0, models[gw.model_kind].vtx_size);

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  // glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

// ----------------------------------------
// init animation
void init_animation(int w, int h)
{
  gw.scrw = w;
  gw.scrh = h;
  gw.angle = 0.0;

  gw.fovy = FOV;
  gw.znear = 1.0;
  gw.zfar = 1000.0;

  gw.model_kind = 0;
  gw.angle_x = 0.0;

  glViewport(0, 0, (int)gw.scrw, (int)gw.scrh);
}

// ----------------------------------------
// Error callback
void error_callback(int error, const char *description)
{
  fprintf(stderr, "Error: %s\n", description);
}

// ----------------------------------------
// Key callback
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
  {
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
    {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key == GLFW_KEY_RIGHT)
    {
      gw.model_kind = (gw.model_kind + 1) % MODELS_LEN;
    }
    else if (key == GLFW_KEY_LEFT)
    {
      gw.model_kind = (gw.model_kind + MODELS_LEN - 1) % MODELS_LEN;
    }
    else if (key == GLFW_KEY_UP)
    {
      gw.angle_x += 15;
    }
    else if (key == GLFW_KEY_DOWN)
    {
      gw.angle_x -= 15;
    }
  }
}

// ----------------------------------------
// window resize callback
static void resize(GLFWwindow *window, int w, int h)
{
  if (h == 0)
    return;

  gw.scrw = w;
  gw.scrh = h;
  glfwSetWindowSize(window, w, h);
  glViewport(0, 0, w, h);
}

// ----------------------------------------
// Main
int main(void)
{
  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
  {
    // Initialization failed
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1); // set OpenGL 1.1
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  // create window
  window = glfwCreateWindow(SCRW, SCRH, WDW_TITLE, NULL, NULL);
  if (!window)
  {
    // Window or OpenGL context creation failed
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);
  glfwSetWindowSizeCallback(window, resize);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Init OpenGL
  int scrw, scrh;
  glfwGetFramebufferSize(window, &scrw, &scrh);
  init_animation(scrw, scrh);

  // main loop
  while (!glfwWindowShouldClose(window))
  {
    render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
