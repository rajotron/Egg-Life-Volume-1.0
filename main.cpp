
#include <cmath>
#include <GL/glut.h>
#include <cstdio>
#include <string>
#include<ctime>
#include <cstdlib>
#include<windows.h>

#define W 640
#define H 390

//Audio Parameters
#define AUDIO_ENABLED
#ifdef AUDIO_ENABLED
#include <AL/al.h>
#include <AL/alc.h>
#include<AL/alut.h>
ALuint buffer[6],source[6];
ALint state[6]={0,0,0,0,0,0};
int audio_enabled=1;
const int no_of_sound_files=7;
const char*filenames[no_of_sound_files]={"chicken-1.wav","chicken-2.wav","chicken-3.wav",
                                         "eggdrop.wav","water-bird2.wav","sample.wav","applause.wav"};
#endif
#ifndef AUDIO_ENABLED
int audio_enabled=0;
#endif

double speed_increasing_scale=audio_enabled?1.0f:1;


//control parameters
float ratio;
float x=0.0f,y=1.75f,z=3500.0f;
float lx=0.0f,ly=0.0f,lz=-1.0f;
float deltaMove = 0;
float basket_x=0,basket_y=-800,basket_z=0;
int paused=0;

//window parameters
int h,w;
int font=(int)GLUT_BITMAP_8_BY_13;
int bitmapHeight=13;
static GLint snowman_display_list;


//other parameters
struct Pos{long x=0,y=1000,z=0;};
Pos hen[3], egg[5];
long x_max=2100, y_fall_max=3200;
int direcx[3]={1,-1,1};
int egg_fall[5]={0,0,0,0,0}, hen_flying_speed=1*speed_increasing_scale;
long double egg_fall_speed=1*speed_increasing_scale;
long score=0, congrscore1=0,congrscore2=0,performance=0, score_rank;
char score_text[20], mytime_text[20], gamelevel_text[20], congrtext[50],perf_text[50], eggsremain_text[50], text[50];
int flag[5]={1,1,1,1,1}, bkgdflag[11]={0,0,0,0,0,0,0,0,0,0,0}, congrflag=0, exit_flag=0,clap_flag=0;       //flags
int gamelevel=0;
long myframe,mytime,mytimebase=0,total_time = 80, time2=0,time1=0; int t=10; //seconds in level 0
char s[30];
long total_eggs=0,neggs1=0,neggs2=1,eggs_to_collect=10,eggs_collected=0;


//Texture
int useMipmaps=1,useTexture=1,linearFiltering=1;
static GLfloat grassVertices[4][3] = {
  { -2000.0, 700.0, 0.0 },
  { 2000.0, 700.0, 0.0 },
  { 2000.0, -2000.0, 0.0 },
  { -2000.0, -2000.0, 0.0 },
};
static char *circles2[] = {
  "....xxxx........",
  "..xxxxxxxx......",
  ".xxxxxxxxxx.....",
  ".xxx....xxx.....",
  "xxx......xxx....",
  "xxx......xxx....",
  "xxx......xxx....",
  "xxx......xxx....",
  ".xxx....xxx.....",
  ".xxxxxxxxxx.....",
  "..xxxxxxxx......",
  "....xxxx........",
  "................",
  "................",
  "................",
  "................",
};

static char *circles[] = {
"................",
"................",
"................",
"................",
"................",
"................",
"...xxxxxxxxxxx..",
"..xxxxxxxxxxxxx.",
"..xxxxxxxxxxxxx.",
"..xxxxxxxxxxxxx.",
"..xxx.xxxxx.xxx.",
"..xx..xxxxx..xx.",
"..xx...xxx...xx.",
"..xx...xxx...xx.",
"..xx...xxx...xx.",
"..x.....x.....x.",
};


//Function Declarations
void initWindow();
void myResize(int w1, int h1);
void processNormalKeys(unsigned char key, int x, int y);
void pressKey(int key, int x, int y);
void releaseKey(int key, int x, int y);
void myGameLevel(int& gamelevel);
void drawSnowMan();
void drawGrassBall(double x, double y, double z);
void drawHen(double x,double y,double z);
void drawEgg(double x,double y,double z);
void mydrawBasket(float x, float y, float z);
void eggFallFunction(int egg_no, int hen_no);
GLuint createDL();
void initScene();
void setOrthographicProjection();
void renderBitmapString(float x, float y, void *font,char *string);
void renderScene(void);
void moveBasket(float i);
void myAllocatePosHens();
void drawBackground();
static void drawPillars(void);
static void drawGrasses(void);
static void makeTexture(void);
void catchEgg();
void myCongratulations(long score,long mytime,int gamelevel);

void bkgddesign_0();
void bkgddesign_1();
void bkgddesign_2();
void bkgddesign_3();
void bkgddesign_4();
void bkgddesign_5();
void bkgddesign_6();
void bkgddesign_7();
void bkgddesign_8();
void bkgddesign_9();
void bkgddesign_10();
void bkgddesign_123();

void gameDescription();
void myStartscreen();
void myEndscreen();
void myReferences();
void myCredits();
void myResult();

#ifdef AUDIO_ENABLED
void initAudio();
void genAudio(ALuint&,ALuint&,const char*);
void playAudio(ALuint&);
ALint audioSourceState(ALuint source);
void audioIntensity(ALuint& source, float newVolume=0.4f);
void delAudio(ALuint&,ALuint&);
void exitAudio();
#endif






int main(int argc, char **argv)
{
    gameDescription();

    srand(time(0));
    myAllocatePosHens();

    #ifdef AUDIO_ENABLED
    initAudio();
    for(int i=0;i<no_of_sound_files;i++){
    genAudio(source[i],buffer[i],filenames[i]);
    }

    audioIntensity(source[0],0.2f);
    audioIntensity(source[1],0.2f);
    audioIntensity(source[2],0.2f);
    audioIntensity(source[3],0.6f);
    audioIntensity(source[4],0.65f);
    audioIntensity(source[5],0.65f);

    delAudio(source[4],buffer[4]);
    genAudio(source[4],buffer[4],filenames[6]);
    delAudio(source[4],buffer[4]);
    genAudio(source[4],buffer[4],filenames[6]);
    playAudio(source[4]);
    clap_flag=1;
    #endif


 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
 glutInitWindowPosition(100,100);
 glutInitWindowSize(W,H);
 glutCreateWindow("Egg Life - Volume 1");

 // register all callbacks
 initWindow();

 glutMainLoop();
 if(gamelevel>0)
 printf("\n\nWell Done! You Cleared %d Level(s) with a Score of %ld.\n",gamelevel,score);
 printf("\nKeep Trying and Score More and More... :) \n\n");
 getchar();
 return(0);
}
void initWindow() {
 glutIgnoreKeyRepeat(1);
 glutKeyboardFunc(processNormalKeys);
 glutSpecialFunc(pressKey);
 glutSpecialUpFunc(releaseKey);
 glutDisplayFunc(renderScene);
 glutIdleFunc(renderScene);
 glutReshapeFunc(myResize);
 initScene();

 glEnable(GL_TEXTURE_2D);
 makeTexture();

 	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set lighting intensity and color
	GLfloat ambientLight[]	= {0.3, 0.3, 0.3, 1.0};
	GLfloat diffuseLight[]	= {1.0, 1.0, 0.0, 1.0};
	GLfloat specularLight[]	= {2.0, 2.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	// Set the light position
	GLfloat qaLightPosition[]	= {2000.0, 3000.0, 6.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

}
void myResize(int w1, int h1)
 {
     //w1=W; h1=H;
     glutReshapeWindow(w1,h1);
     if(h1==0) h1=1;
 w = w1;
 h = h1;
 ratio = 1.0f * w / h;
 // Reset the coordinate system before modifying
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();

 // Set the view port to be the entire window
    glViewport(0, 0, w, h);

 // Set the clipping volume
 gluPerspective(45,ratio,0.1,10000);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(x, y, z,
        x + lx,y + ly,z + lz,
     0.0f,1.0f,0.0f);
 }


void processNormalKeys(unsigned char key, int x, int y) {

  switch (key)
   {
    case 27 :if(gamelevel>0)
              printf("\n\nWell Done! You Cleared %d Level(s) with a Score of %ld.\n",gamelevel,score);
              printf("\nKeep Trying and Score More and More... :) \n\n");
             exit(0);
    case ' ': paused=abs(paused-1);break;//Pause
    }
}

void pressKey(int key, int x, int y) {

 switch (key) {
    case GLUT_KEY_LEFT : if(basket_x > 2100) basket_x= 2100;if(basket_x < -2100) basket_x=-2100;
                         deltaMove = 6.0*speed_increasing_scale;break;
    case GLUT_KEY_RIGHT : if(basket_x > 2100) basket_x= 2100;if(basket_x < -2100) basket_x=-2100;
                          deltaMove = -6.0*speed_increasing_scale;break;
  //case GLUT_KEY_LEFT : deltaAngle = -0.02f;break;
  //case GLUT_KEY_RIGHT : deltaAngle = 0.02f;break;
  //case GLUT_KEY_END: if(ly < 0.5) deltaAngleUp= 0.01f;printf(" deltaAngleUp = %f ; ly = %f\n ",deltaAngleUp,ly);break;
  //case GLUT_KEY_INSERT: if(ly > -0.5) deltaAngleUp = -0.01f;printf(" deltaAngleUp = %f ; ly = %f\n ",deltaAngleUp,ly);break;
 }

}

void releaseKey(int key, int x, int y) {

 switch (key) {
  case GLUT_KEY_LEFT :  if (deltaMove > 0)
         deltaMove = 0;
        break;
  case GLUT_KEY_RIGHT : if (deltaMove < 0)
         deltaMove = 0;
        break;
  /*
  case GLUT_KEY_LEFT : if (deltaAngle < 0.0f) deltaAngle = 0.0f;break;
  case GLUT_KEY_RIGHT : if (deltaAngle > 0.0f) deltaAngle = 0.0f;break;
  case GLUT_KEY_END: deltaAngleUp =0.0f;break;
  case GLUT_KEY_INSERT: deltaAngleUp = 0.0f;break;
  */
 }
}




void myGameLevel(int& gamelevel)
 {

     //Game Level 1
  if(gamelevel>-1){int n=0;
  if(hen[n].x>=x_max)direcx[n]=-1;
  else if(hen[n].x<=-x_max)direcx[n]=1;
  hen[n].x=hen[n].x+direcx[n]*1*hen_flying_speed;
  drawHen(hen[n].x,hen[n].y,hen[n].z);
  eggFallFunction(0,n);
  #ifdef AUDIO_ENABLED
  if(state[0]==AL_PLAYING);else playAudio(source[0]);
            state[0]=audioSourceState(source[0]);
  #endif
     }
     //Game Level 2
 if(gamelevel>0){
   eggFallFunction(1,0);
     }
     //Game Level 3
 if(gamelevel>1){
   eggFallFunction(2,0);
     }

    //Game Level 4
 if(gamelevel>3){int n=1;
  if(hen[n].x>=x_max)direcx[n]=-1;
  else if(hen[n].x<=-x_max)direcx[n]=1;
  hen[n].x=hen[n].x+direcx[n]*2*hen_flying_speed;
  drawHen(hen[n].x,hen[n].y,hen[n].z);
  eggFallFunction(3,n);
  #ifdef AUDIO_ENABLED
  if(state[1]==AL_PLAYING);else playAudio(source[1]);
            state[1]=audioSourceState(source[1]);
            #endif
     }
     //Game Level 5
  if(gamelevel>4){
   eggFallFunction(4,1);
   //Game Level 6
  if(gamelevel>5){int n=2;
  if(hen[n].x>=x_max)direcx[n]=-1;
  else if(hen[n].x<=-x_max)direcx[n]=1;
  hen[n].x=hen[n].x+direcx[n]*2.5*hen_flying_speed;
  drawHen(hen[n].x,hen[n].y,hen[n].z);
  eggFallFunction(4,n);
  #ifdef AUDIO_ENABLED
  if(state[2]==AL_PLAYING);else playAudio(source[2]);
            state[2]=audioSourceState(source[2]);
  #endif
     }
  }

  if(gamelevel==0){egg_fall_speed=3;eggs_to_collect=20;}
  if(gamelevel==1){egg_fall_speed=3;eggs_to_collect=20;}
  if(gamelevel==2){egg_fall_speed=3;eggs_to_collect=30;}
  if(gamelevel==3){egg_fall_speed=3;eggs_to_collect=30;}
  if(gamelevel==4){egg_fall_speed=3;eggs_to_collect=30;}
  if(gamelevel==5){egg_fall_speed=3;eggs_to_collect=40;}
  if(gamelevel==6){egg_fall_speed=3;eggs_to_collect=40;}
  if(gamelevel==7){hen_flying_speed=2;egg_fall_speed=3;eggs_to_collect=50;}
  if(gamelevel==8){hen_flying_speed=2;egg_fall_speed=3;eggs_to_collect=50;}
  if(gamelevel==9){hen_flying_speed=2;egg_fall_speed=3;eggs_to_collect=50;}
  if(gamelevel==10){hen_flying_speed=2;egg_fall_speed=4;eggs_to_collect=60;}
  if(gamelevel==11){hen_flying_speed=2;egg_fall_speed=4;eggs_to_collect=80;}
  if(gamelevel>11){hen_flying_speed=2;egg_fall_speed=5;eggs_to_collect=100;}

 }

/*void drawSnowMan() {

// Set material properties
	GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat qaGreen[] = {0.5, 0.0, 0.0, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaGreen);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaGreen);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0);


// Draw Body
 glTranslatef(0.0f ,0.75f, 0.0f);
 glutSolidSphere(0.75f,20,20);


// Draw Head
 glTranslatef(0.0f, 1.0f, 0.0f);
 glutSolidSphere(0.25f,20,20);

// Draw Eyes
 glPushMatrix();
 glColor3f(0.0f,0.0f,0.0f);
 glTranslatef(0.05f, 0.10f, 0.18f);
 glutSolidSphere(0.05f,10,10);
 glTranslatef(-0.1f, 0.0f, 0.0f);
 glutSolidSphere(0.05f,10,10);
 glPopMatrix();

// Draw Nose
 glColor3f(1.0f, 0.5f , 0.5f);
 glRotatef(0.0f,1.0f, 0.0f, 0.0f);
 glutSolidCone(0.08f,0.5f,10,2);
}
*/
void drawGrassBall(double x, double y, double z)
{
    glutSolidSphere(0.5f,16,16);
}

void drawHen(double x,double y,double z)
{
    GLfloat qaBlack[] = {0.8, 0.8, 0.0, 1.0};
	GLfloat qaGreen[] = {0.5, 0.0, 0.0, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaBlack);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaGreen);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0);

    glPushMatrix();
    glTranslated(x,y,z);
    glutSolidSphere(100,16,16);
    glPopMatrix();

    glPushMatrix();
    GLfloat qa2Black[] = {0.5, 0.4, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qa2Black);
    glTranslated(x,y+80,z+80);
    glutSolidSphere(50,16,16);
    glPopMatrix();

    glPushMatrix();
    GLfloat qa3Black[] = {0, 0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qa3Black);
    glTranslated(x,y+50,z+100);
    glRotated(50,1,0,0);
    glutSolidCone(25,85,16,16);
    glPopMatrix();

}

void drawEgg(double x,double y,double z)
{
    GLfloat qaBlack[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat qaGreen[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaGreen);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaGreen);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0);

    glPushMatrix();
    glTranslated(x,y,z);
    glutSolidSphere(25,16,16);
    glPopMatrix();

}

void drawBasket(float x, float y, float z)
{
    GLfloat qaBlack[] = {0.9, 0.3, 0.0, 1.0};
	GLfloat qaGreen[] = {0.8, 0.8, 0.0, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaBlack);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaGreen);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0);

    glPushMatrix();

    glTranslatef(x,y,z);
    glutSolidCube(200);
    glTranslatef(0,y+900,0);
    glRotatef(90,1,0,0);
    glutSolidTorus(100,120,16,16);

    glPopMatrix();


}

void eggFallFunction(int egg_no,int hen_no )
{
  if(mytime%1==0 || /*mytime%500==7 || mytime%1100==0 ||*/ egg_fall[egg_no])
  {

    if(egg[egg_no].y>=-y_fall_max)
    {
        egg_fall[egg_no]=1;
        if(flag[egg_no]){egg[egg_no]=hen[hen_no];flag[egg_no]=0;total_eggs+=1;}
        drawEgg(egg[egg_no].x,egg[egg_no].y,egg[egg_no].z);
        egg[egg_no].y=egg[egg_no].y-egg_fall_speed;
     }
    else { egg_fall[egg_no]=0; egg[egg_no].y=500;flag[egg_no]=1;}
  }
}

GLuint createDL() {
 GLuint snowManDL;

 // Create the id for the list
 snowManDL = glGenLists(5);

 glNewList(snowManDL+1,GL_COMPILE);
  drawGrassBall(1,1,1);//drawSnowMan();//drawDinosaur();//
 glEndList();
 // start list
 glNewList(snowManDL,GL_COMPILE);

 // call the function that contains the rendering commands
 for(int i = -6; i < 6; i++)
  for(int j=-6; j < 6; j++) {
   glPushMatrix();
   glTranslatef(i*10.0,0,j * 10.0);
   glCallList(snowManDL+1);
   glPopMatrix();
  }

 // endList
 glEndList();

 return(snowManDL);
}

void initScene() {

 glEnable(GL_DEPTH_TEST);
 snowman_display_list = createDL();

}


void setOrthographicProjection() {

 // switch to projection mode
 glMatrixMode(GL_PROJECTION);
 // save previous matrix which contains the
 //settings for the perspective projection
 glPushMatrix();
 // reset matrix
 glLoadIdentity();
 // set a 2D orthographic projection
 gluOrtho2D(0, w, 0, h);
 // invert the y axis, down is positive
 glScalef(1, -1, 1);
 // mover the origin from the bottom left corner
 // to the upper left corner
 glTranslatef(0, -h, 0);
 glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
 // set the current matrix to GL_PROJECTION
 glMatrixMode(GL_PROJECTION);
 // restore previous settings
 glPopMatrix();
 // get back to GL_MODELVIEW matrix
 glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, void *font,char *string)
{

  char *c;
  // set position to start drawing fonts
  glRasterPos2f(x, y);
  // loop all the characters in the string
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}


void renderScene(void) {
if(exit_flag){paused=1;}
if(!paused)
{
 if (deltaMove)
  moveBasket(deltaMove);

  total_time+=(time2-time1)/1000;

 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Draw background
drawPillars();
drawGrasses();

    drawBackground();



    drawBasket(basket_x,basket_y,basket_z);

// Draw 36 Snow Men
//glCallList(snowman_display_list);

 myGameLevel(gamelevel);
 catchEgg();

 myframe++;
 mytime=glutGet(GLUT_ELAPSED_TIME);
 if (mytime - mytimebase > 1000) {
  sprintf(s,"FPS:%4.2f",myframe*1000.0/(mytime-mytimebase));
  mytimebase = mytime;
  myframe = 0;
 }
 sprintf(score_text,"Score: %ld",score);
 sprintf(mytime_text,"Time : %ld sec",total_time-mytime/1000);
 sprintf(gamelevel_text,"Level: %ld",gamelevel);
 sprintf(perf_text,"Performance: %d/20",performance);
 sprintf(eggsremain_text,"Eggs to collect: %ld",eggs_to_collect-eggs_collected);

time1=time2=mytime;



 GLfloat AmbientColor[] = {6.0, 6.0, 6.0, 1.0};
 glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
 setOrthographicProjection();
 glPushMatrix();
 glLoadIdentity();font=2;


 if((eggs_to_collect-eggs_collected)<=0)
 {
    int font1=5;
    sprintf(text,"WELL DONE!! YOU PASSED LEVEL %d",gamelevel);

 }
 else if((int)(total_time-mytime/1000)<=0&&(eggs_to_collect-eggs_collected)>0)
 { int font1=5;
    sprintf(text,"Ooops!%ld eggs were remaining! Good Luck Next Time :)",eggs_to_collect-eggs_collected);
    renderBitmapString(w/5,h/3+15,(void *)font,text);
    if(gamelevel>0)
      printf("\n\nWell Done! You Cleared %d Level(s) with a Score of %ld.\n",gamelevel,score);
      printf("\nKeep Trying and Score More and More... :) \n\n");
   if(exit_flag){paused=1;}
 }else
 {
     //sprintf(text," "," ");

 }
 #ifdef AUDIO_ENABLED
  if(clap_flag)
{
  state[4]=audioSourceState(source[4]);
  if(state[4]==AL_PLAYING);
  else
  {
      delAudio(source[4],buffer[4]);
      genAudio(source[4],buffer[4],filenames[4]);
      delAudio(source[4],buffer[4]);
      genAudio(source[4],buffer[4],filenames[4]);
      audioIntensity(source[4],0.75f);
      playAudio(source[4]);
      clap_flag=0;
  }
}
 #endif // AUDIO_ENABLED

 renderBitmapString(10,15,(void *)font,score_text);
 renderBitmapString(10,35,(void *)font,gamelevel_text);
 renderBitmapString(10,75,(void *)font,eggsremain_text);
 renderBitmapString(10,95,(void *)font,perf_text);
 renderBitmapString(10,115,(void *)font,"Spacebar - Pause");
 renderBitmapString(10,135,(void *)font,"Esc - Quit");
 renderBitmapString(10,h-10,(void *)font,s);
 renderBitmapString(w/4,h-20,(void *)font,text);



 if((int)(total_time-mytime/1000)<=10)
{

        #ifdef AUDIO_ENABLED
         if(state[5]==AL_PLAYING);else playAudio(source[5]);
            state[5]=audioSourceState(source[5]);
        #endif // AUDIO_ENABLED

  GLfloat AmbientColor[] = {6.0, 0.0, 0.0, 1.0};
  glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
  renderBitmapString(10,55,(void *)font,mytime_text);
  //t--;
}else
{

    renderBitmapString(10,55,(void *)font,mytime_text);
}

 myCongratulations(score,mytime,gamelevel);

 if((int)(total_time-mytime/1000)<=0||(eggs_to_collect-eggs_collected)<=0)
 {

    myResult();

 }

 glPopMatrix();

 resetPerspectiveProjection();

 glutSwapBuffers();

 }
 else
 {
     time2=glutGet(GLUT_ELAPSED_TIME);

 }
}

void moveBasket(float i) {
 //glLoadIdentity();
 basket_x=basket_x-i;
 drawBasket(basket_x,basket_y,basket_z);

}

void myAllocatePosHens()
{
    hen[0].x=0;hen[0].y=1000;hen[0].z=0;
    hen[1].x=2000;hen[1].y=950;hen[1].z=0;
    hen[2].x=-2000;hen[2].y=1100;hen[2].z=0;
}

void drawBackground()
{
    //if(bkgdflag[gamelevel]) goto last;
    switch(gamelevel)
    {
    case 0:bkgddesign_0();
           bkgdflag[gamelevel]++;break;
    case 1:bkgddesign_1();
           glClearColor(0.1,0.0,0.0,1);
           bkgdflag[gamelevel]++;break;
    case 2:bkgddesign_2();
           bkgdflag[gamelevel]++;break;
    case 3:bkgddesign_3();
           bkgdflag[gamelevel]++;break;
    case 4:bkgddesign_4();
           bkgdflag[gamelevel]++;break;
    case 5:bkgddesign_5();
           bkgdflag[gamelevel]++;break;
    case 6:bkgddesign_6();
           bkgdflag[gamelevel]++;break;
    case 7:bkgddesign_7();
           bkgdflag[gamelevel]++;break;
    case 8:bkgddesign_8();
           bkgdflag[gamelevel]++;break;
    case 9:bkgddesign_9();
           bkgdflag[gamelevel]++;break;
   case 10:bkgddesign_10();
           bkgdflag[gamelevel]++;break;
   default:bkgddesign_123();
    }
    last: ;
}

static void
makeTexture(void)
{
  GLubyte pillarTexture[16][16][3];
  GLubyte grassTexture[16][16][3];
  GLubyte *loc;
  int s, t;

  /* Setup RGB image for the texture. */
  loc = (GLubyte*) pillarTexture;
  for (t = 0; t < 16; t++) {
    for (s = 0; s < 16; s++) {
      if (circles[t][s] == 'x') {
	/* Nice green. */
        loc[0] = 0x1f;  //0x1f;
        loc[1] = 0x8f;  //0x8f;
        loc[2] = 0x1f;  //0x1f;
      } else {
	/* Light brown. */
        loc[0] = 0x80;  //loc[0] = 0xaa;
        loc[1] = 0x30;  //loc[0] = 0xaa;
        loc[2] = 0x00;  //loc[0] = 0xaa;
      }
      loc += 3;
    }
  }


  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  if (useMipmaps) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      GL_LINEAR_MIPMAP_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 16, 16,
      GL_RGB, GL_UNSIGNED_BYTE, pillarTexture);

  } else {
    if (linearFiltering) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0,
      GL_RGB, GL_UNSIGNED_BYTE, pillarTexture);
  }
}

static void drawPillars()
{
    glDisable(GL_LIGHTING);

  if (useTexture) {
    glEnable(GL_TEXTURE_2D);
  }

  glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(-2200,-3000,0);
    glTexCoord2f(0.0, 16.0);
    glVertex3f(-2200,800,0);
    glTexCoord2f(16.0, 16.0);
    glVertex3f(-2000,800,0);
    glTexCoord2f(16.0, 0.0);
    glVertex3f(-2000,-3000,0);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(2200,-3000,0);
    glTexCoord2f(0.0, 16.0);
    glVertex3f(2200,800,0);
    glTexCoord2f(16.0, 16.0);
    glVertex3f(2000,800,0);
    glTexCoord2f(16.0, 0.0);
    glVertex3f(2000,-3000,0);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(-2200,700,0);
    glTexCoord2f(0.0, 16.0);
    glVertex3f(2200,700,0);
    glTexCoord2f(16.0, 16.0);
    glVertex3f(2200,600,0);
    glTexCoord2f(16.0, 0.0);
    glVertex3f(-2200,600,0);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(-2500,-2800,0);
    glTexCoord2f(0.0, 16.0);
    glVertex3f(-2400,000,0);
    glTexCoord2f(16.0, 16.0);
    glVertex3f(2400,000,0);
    glTexCoord2f(16.0, 0.0);
    glVertex3f(2500,-2800,0);

  glEnd();

  if (useTexture) {
    glDisable(GL_TEXTURE_2D);
  }

  glEnable(GL_LIGHTING);
}

static void drawGrasses()
{
  /*  glDisable(GL_LIGHTING);

  if (useTexture) {
    glEnable(GL_TEXTURE_2D);
  }

  glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(-2200,-1000,0);
    glTexCoord2f(0.0, 16.0);
    glVertex3f(-2200,1000,0);
    glTexCoord2f(16.0, 16.0);
    glVertex3f(2200,1000,0);
    glTexCoord2f(16.0, 0.0);
    glVertex3f(2200,1000,0);

  glEnd();

  if (useTexture) {
    glDisable(GL_TEXTURE_2D);
  }

  glEnable(GL_LIGHTING);*/
}



void catchEgg()
{

  for(int i=0;i<5;i++){

  if((abs(egg[i].x-basket_x)<200) && (abs(egg[i].y-basket_y-100)<5))
        {
            #ifdef AUDIO_ENABLED
            playAudio(source[3]);
            #endif // AUDIO_ENABLED
            eggs_collected++;
            score++;egg[i].y=-y_fall_max;
        }
  }

}
void myCongratulations(long score, long time_elapsed, int gamelevel )
{
    congrscore2=score;
    neggs2=total_eggs;

    if(neggs2-neggs1>=20)
    {
    performance=congrscore2-congrscore1;
    if(performance>=20)
    {
        font=5;sprintf(congrtext,"YOUR PERF: %d/20  EXTRAORDINARILY BRILLIANT!!!!",performance);
    }
    else if(performance<15&&performance>=8)sprintf(congrtext,"Your performance is Good! Keep trying :)"," ");
    else if(performance<8)sprintf(congrtext," "," ");

    switch(performance)
    {
      case 19:font=5;sprintf(congrtext,"YOUR PERF: %d/20  BRILLIANT!!!",performance);break;
      case 18:font=5;sprintf(congrtext,"YOUR PERF: %d/20  EXCELLENT!!!",performance);break;
      case 17:font=5;sprintf(congrtext,"YOUR PERF: %d/20  VERY GOOD!!",performance);break;
      case 16:font=5;sprintf(congrtext,"YOUR PERF: %d/20  GREAT!",performance);break;
      case 15:font=5;sprintf(congrtext,"YOUR PERF: %d/20  GOOD!",performance);break;
    }
    congrscore1=congrscore2;
    neggs1=neggs2;
    }
if(neggs2-neggs1<=4){
    GLfloat AmbientColor[] = {0.0, 0.8, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
    renderBitmapString(w/4,h/3+3,(void *)font,congrtext);
    }


}

void bkgddesign_0(){
glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();

}

void bkgddesign_1(){
glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

   //float a1=(float)(rand()%10)/10;
   //float a2=(float)(rand()%10)/10;
   //float a3=(float)(rand()%10)/10;
	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}

void bkgddesign_2(){
glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}

void bkgddesign_3(){
    glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}

void bkgddesign_4(){
    glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}

void bkgddesign_5(){
    glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}

void bkgddesign_6(){
    glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}

void bkgddesign_7(){
    glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}

void bkgddesign_8(){
    glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}

void bkgddesign_9(){
    glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}

void bkgddesign_10(){
    glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED

	GLfloat AmbientColor[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}

void bkgddesign_123(){
    glClearColor(0.3,0.3,0.5,1);
#ifdef AUDIO_ENABLED
  if(state[4]==AL_PLAYING);else playAudio(source[4]);
            state[4]=audioSourceState(source[4]);
#endif // AUDIO_ENABLED


//float a1,a2,a3;
//a1=(float)(score%100)/100;
//a2=(float)(mytime%100)/100;
//a3=(float)(total_eggs%100)/100;
	GLfloat AmbientColor[] ={0.0, 0.0, 3.0, 1.0};
	GLfloat DiffuseColor[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat SpecularColor[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

	// Draw square with many little squares
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		const GLfloat bkdgDelta = 400;
		for (int i = -10; i < 10; ++i) {
			for (int j = -10; j < 10; ++j) {
				glVertex3f(j*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, i*bkdgDelta, 0);
				glVertex3f((j+1)*bkdgDelta, (i+1)*bkdgDelta, 0);
				glVertex3f(j*bkdgDelta, (i+1)*bkdgDelta, 0);
			}
		}

	glEnd();
	}



void myResult()
{

    if(eggs_to_collect-eggs_collected<=0)
    {

        #ifdef AUDIO_ENABLED
        delAudio(source[4],buffer[4]);
        genAudio(source[4],buffer[4],filenames[5]);
        delAudio(source[4],buffer[4]);
        genAudio(source[4],buffer[4],filenames[6]);
        audioIntensity(source[4],0.65f);
        playAudio(source[4]);
        clap_flag=1;
        #endif // AUDIO_ENABLED


       total_time+=40;
        gamelevel++;
        eggs_collected=0;

    }
    else if (eggs_to_collect-eggs_collected>1)
             {
                 exit_flag=1;
             }

}





////////////*================Audio Functions===========================*////////
#ifdef AUDIO_ENABLED

void initAudio()
{
    if(audio_enabled)
   {
    // Initialize the environment
    alutInit(0, NULL);

    // Capture errors
    alGetError();
   }
}

void genAudio(ALuint& source,ALuint& buffer, const char*filename)
{
     // Load pcm data into buffer
    buffer = alutCreateBufferFromFile(filename);

    // Create sound source (use buffer to fill source)
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
}

void playAudio(ALuint& source)
{
    // Play
   if(audio_enabled)
   {alSourcePlay(source);//source);
    //Sleep(3);
    //alSourcePause(source);
   }

    // Wait for the song to complete
   /* do {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    } while (state == AL_PLAYING);
    */
}

ALint audioSourceState(ALuint source)
{
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state;
}

void audioIntensity(ALuint& source, float newVolume)
{
  alSourcef(source, AL_GAIN, newVolume);
}

void delAudio(ALuint& source,ALuint& buffer)
{
  if(audio_enabled)
  {// Clean up sources and buffers
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    }

}

void exitAudio()
{
    // Exit everything
    alutExit();
}
#endif // AUDIO_ENABLED


void gameDescription()
{
    printf("\t\t\t  Game: EGG LIFE - Volume 1\n\n");
    printf("In this single player game, the player\nhas to catch eggs laid by hens flying overhead.\n\As the player progresses through the game, the game\n will become more and more difficult.\n\
            \nAt the top left, player's current score, level,\n remaining time, remaining eggs, and performance( in scale of 20) are given. \n    If the current level is passed, EXTRA 40 seconds are\n added to the total time remaining.\n\
            \n\n\n\t\t Let's see how many levels you can clear...  (press ENTER)");

  getchar();
}
