//----------------------------------------------------------------------------
// opengl-tutorial03
// + changes to allow multiple triangles with different colors
// + added right triangle, square, cube
//
// Kambria Rogalski kambriar@udel.edu
// 2025
//----------------------------------------------------------------------------


// Include standard headers

#include <stdio.h>
#include <stdlib.h>

#include <cmath>

#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp> //matrix transformations
using namespace glm;


#include <common/shader.hpp>
#include <common/controls.hpp>  // Include for camera control functions

//----------------------------------------------------------------------------


// these along with Model matrix make MVP transform


glm::mat4 Projection;
glm::mat4 View;


// some globals necessary to get information to shaders


GLuint MatrixID;
GLuint vertexbuffer;
GLuint colorbuffer;


void draw_triangle(glm::mat4, float, float, float);
void draw_right_triangle(glm::mat4, float, float, float);
void draw_square(glm::mat4, float, float, float);
void draw_cube(glm::mat4, float, float, float);
void draw_pyr3(glm::mat4, float, float, float, float h=1);
void draw_stella(glm::mat4, float, float, float);

//----------------------------------------------------------------------------

// void draw_octahedron(glm::mat4 Model, float r, float g, float b)
// {

// }

// 2 x 2 x 2 cube centered on (0, 0, 0)


void draw_cube(glm::mat4 Model, float r, float g, float b)
{
  // +Z, -Z

  draw_square(Model * glm::translate(glm::vec3(0.0f, 0.0f, +1.0f)), r, g, b);
  draw_square(Model * glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)), 0.5*r, 0.5*g, 0.5*b);


  // +X, -X


  glm::mat4 RY = glm::rotate((float) (0.5*M_PI),
                             glm::vec3(        0.0f,
                                        1.0f,
                                        0.0f));


  draw_square(Model * glm::translate(glm::vec3(+1.0f, 0.0f, 0.0f)) * RY, g, b, r);
  draw_square(Model * glm::translate(glm::vec3(-1.0f, 0.0f, 0.0f)) * RY, 0.5*g, 0.5*b, 0.5*r);


  // +Y, -Y


  glm::mat4 RX = glm::rotate((float) (0.5*M_PI),
                             glm::vec3(        1.0f,
                                        0.0f,
                                        0.0f));


  draw_square(Model * glm::translate(glm::vec3(0.0f, +1.0f, 0.0f)) * RX, b, r, g);
  draw_square(Model * glm::translate(glm::vec3(0.0f, -1.0f, 0.0f)) * RX, 0.5*b, 0.5*r, 0.5*g);


}


//----------------------------------------------------------------------------


// 2 x 2 square centered on (0, 0)


void draw_square(glm::mat4 Model, float r, float g, float b)
{
  glm::mat4 M = glm::scale(glm::vec3(-1.0f, -1.0f, 0.0f));


  //  draw_right_triangle(Model * M, 1.0-r, 1.0-g, 1.0-b);
  draw_right_triangle(Model * M, r, g, b);
  draw_right_triangle(Model, r, g, b);
}


//----------------------------------------------------------------------------

void draw_pyr3(glm::mat4 Model, float r, float g, float b, float h){
    double r3o3 = std::sqrt(3)/3; //root 3 over 3
    double angle = (0.5*M_PI - acos(r3o3/sqrt(3))); //90 degrees - cos^-1 of adjacent(r3o3) over hypotenuse aka slant height (sqrt(3))

    glm::mat4 rot = glm::rotate((float) angle, glm::vec3(-1.0f, 0.0f, 0.0f)); //rotate to dihedral angle
    glm::mat4 trans = glm::translate(glm::vec3(0.0f, -sin(angle)*r3o3 ,cos(angle)*r3o3)); // translate to edge w/ trig
    for (int i=0;i<3;i++)
        draw_triangle(Model * glm::rotate(glm::radians(120.0f*i), glm::vec3(0.0f, 1.0f, 0.0)) // rotate around y
                          * rot
                          * trans
                        //* glm::scale(glm::vec3(1.0f, cos(angle)*h, sin(angle)*h))
                          , r
                          , g+i/3.0
                          , b);


    /*
    //Face  RED
    draw_triangle(Model * rot//glm::rotate((float) angle, glm::vec3(-1.0f, 0.0f, 0.0f)) // Rotate 180 around Y
                        * trans//glm::translate(glm::vec3(0.0f, -sin(angle)*r3o3 ,cos(angle)*r3o3)) // Adjusted translation
                        // glm::scale(glm::vec3(1.0f, cos(angle)*h, sin(angle)*h))
                        , 1.0f, g, b);

    //Face 2 YELLOW
    draw_triangle(Model * glm::rotate(glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0))
                        * rot//glm::rotate((float) angle, glm::vec3(-1.0f, 0.0f, 0.0f))
                        * trans//glm::translate(glm::vec3(0.0f, -sin(angle)*r3o3 ,cos(angle)*r3o3)) // Adjusted translation
                        // glm::scale(glm::vec3(1.0f, cos(angle)*h, sin(angle)*h))
                        , r, 1.0f, b);


    //Face 3 PINK
    draw_triangle(Model * glm::rotate(glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f))
                        * rot//glm::rotate((float) angle, glm::vec3(-1.0f, 0.0f, 0.0f))
                        * trans//glm::translate(glm::vec3(0.0f, -sin(angle)*r3o3 ,cos(angle)*r3o3)) // Adjusted translation
                        // glm::scale(glm::vec3(1.0f, cos(angle)*h, sin(angle)*h))
                        , r, g, 1.0f);
    */

}


void draw_stella(glm::mat4 Model, float r, float g, float b){
    double r3o3 = sqrt(3)/3;
    double angle = acos(1/sqrt(3));
    glm::mat4 rot = glm::rotate((float) angle, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 trans1 = glm::translate(glm::vec3(0.0f, sin(angle), cos(angle)-r3o3));
    for(int i=0;i<8;i++){
        draw_pyr3(Model
                      * glm::rotate(glm::radians((i/4) *180.0f ), glm::vec3(1.0f, 0.0f, 0.0f))
                      * glm::rotate(glm::radians(90.0f*i), glm::vec3(0.0f, 1.0f, 0.0f))
                      * rot
                      * trans1, r+.25*(i/4), g, b+i/8.0);
    }
}



// with shear, bottom-left at (-1, -1), bottom-right at (1, -1),
// top-right at (1, 1)


void draw_right_triangle(glm::mat4 Model, float r, float g, float b)
{
  glm::mat4 S = glm::shearX3D (glm::mat4(1.0f), 0.5f, 0.0f);
  glm::mat4 T = glm::translate(glm::vec3(-1.0f, 1.0f, 0.0f));

  draw_triangle(Model * glm::inverse(T) * S * T, r, g, b);
}


//----------------------------------------------------------------------------


// bottom-left at (-1, -1), bottom-right at (1, -1),
// top at (0, 1)


// Draw triangle with particular modeling transformation and color (r, g, b) (in range [0, 1])
// Refers to globals in section above (but does not change them)


void draw_triangle(glm::mat4 Model, float r, float g, float b)
{
  // Our ModelViewProjection : multiplication of our 3 matrices
  glm::mat4 MVP = Projection * View * Model;


  // make this transform available to shaders
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(0,                  // attribute. 0 to match the layout in the shader.
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        0,                  // stride
                        (void*)0            // array buffer offset
                        );

  // all vertices same color


  GLfloat g_color_buffer_data[] = {
    r, g, b,
    r, g, b,
    r, g, b,
  };

  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

  // 2nd attribute buffer : colors
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer(1,                                // attribute. 1 to match the layout in the shader.
                        3,                                // size
                        GL_FLOAT,                         // type
                        GL_FALSE,                         // normalized?
                        0,                                // stride
                        (void*)0                          // array buffer offset
                        );


  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}


//----------------------------------------------------------------------------


int main( void )
{
  // Initialise GLFW
  if( !glfwInit() )
    {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      getchar();
      return -1;
    }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

  // Open a window and create its OpenGL context
  window = glfwCreateWindow( 1024, 768, "Tutorial 03 - Matrices", NULL, NULL);
  if( window == NULL ){
    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
    getchar();
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return -1;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Darker blue background
  glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
  glEnable(GL_DEPTH_TEST);


  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders( "MultiColorSimpleTransform.vertexshader", "MultiColor.fragmentshader" );

  // Get a handle for our "MVP" uniform
  MatrixID = glGetUniformLocation(programID, "MVP");

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  // Or, for an ortho camera :
  //  Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In camera coordinates
  //  Projection = glm::ortho(-3.0f,3.0f,-3.0f,3.0f,0.0f,100.0f); // In camera coordinates

  // Camera matrix -- same for all triangles drawn
  // View       = glm::lookAt(glm::vec3(40,40,40), // Camera is at (4,3,3), in World Space
  //                          glm::vec3(0,0,0), // and looks at the origin
  //                          glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
  //                          );

  //Camera matrix for Plan View (top-down view)
  // View = glm::lookAt(glm::vec3(0, 70, 0), // Camera is above at (0, 70, 0)
  //                    glm::vec3(0, 0, 0),  // Camera is looking at the origin (0, 0, 0)
  //                    glm::vec3(0, 0, 1)); // Up is along the positive Z-axis

  //Camera matrix for Isometric View
  View = glm::lookAt(glm::vec3(40, 40, 40), // Camera at (10, 10, 10), above and diagonally offset
                   glm::vec3(0, 0, 0),  // Camera is looking at the origin
                   glm::vec3(0, 1, 0));  // Up is along the positive Y-axis


  // Front view
  // View       = glm::lookAt(glm::vec3(0,0,5)
  //                    glm::vec3(0,0,0)
  //                    glm::vec3(0,1,0)
  //                    );
  // Side view:
  // View       = glm::lookAt(glm::vec3(5,0,0),
  //                    glm::vec3(0,0,0),
  //                    glm::vec3(0,1,0)
  //                    );

  // geometry of "template" triangle


  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f,
     0.0f, std::sqrt(3), 0.0f,
  };
  static const GLushort g_element_buffer_data[] = { 0, 1, 2 };

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  // handle for color information.  we don't set it here because it can change for each triangle


  glGenBuffers(1, &colorbuffer);

  // Model matrix -- changed for each triangle drawn
  glm::mat4 Model;

  bool grow = true;
  double value = 6;
  double min = 0;
  double max = 25;
  double rate = 0.005;

  do{

      if (grow){
          value+= rate;
          if (value >= max)
              grow = false;
      }
      else{
          value -= rate;
          if (value <= min)
              grow = true;
      }

    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    // set model transform and color for each triangle and draw it offscreen


    //    move cube up and stretch it a bit
    //draw_cube(glm::scale(glm::vec3(1.0f, 2.0f, 1.0f)) * glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)),
                //1.0f, 0.0f, 0.0f);   // red

    //draw_triangle(glm::mat4(), 1.0f, 0.0f, 0.0f);
    //draw_pyr3(glm::mat4(1.0f)/* * glm::translate(glm::vec3(0.0f,0.0f,-sqrt(3)/3))*/, 1.0f, 0.75f, 0.75f);
    //draw_stella(glm::mat4(), 0.5f, 0.05f, 0.05f);
    double radius=30;
    int iterations = 100;
    double rotations = value;
    for (int i=0;i<iterations;i++){
        double phi=glm::radians(-90.0+180.0/iterations*i);
        double theta = glm::radians(0+360.0*rotations/iterations*i);
        double xyRadius = cos(phi)*radius;
        draw_stella(Model
                    *glm::translate(glm::vec3(xyRadius*cos(theta), sin(phi)*radius, xyRadius*sin(theta)))
                    , 0.5f/iterations*i, 0.5f-iterations*0.5/i, 0.5f);
    }

    //draw_triangle(glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)), 0.0f, 1.0f, 0.0f);

    // more/other calls to draw_triangle() ...


    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();


  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0 );

  // Cleanup VBO and shader
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &VertexArrayID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
