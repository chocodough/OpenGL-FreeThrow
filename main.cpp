#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <glut.h>

#define timerTerm 10

int Width = 1000, Height = 1000;
double AngleX = -10;
double AngleY = 0;
double Zoom = -22.0;
int t = 0; //시간기준, 1초에 100증가
double wx = 0, wy = 10, wz = -20;  //백보드 좌표
double bx, by, bz;  //공 중심 좌표
double rx, ry, rz;  //림 중심 좌표, 반지름
double v = 0;// 공의 속도
double vx,vy,vz;//각 좌표별 공의 속도
double ro = 0;//방향
int fire = 0;//공의 발사 여부
double pi = 3.1415;//원주율
double tt = 0; //던지기 시작한 다음부터의 시간
double st, et = 0;//클릭을 한 시간과 뗀 시간
int ms = 1; //클릭여부
double tempro = 0;//마우스를 움직이는 동안의 각도를 임시로 저장
double a = 0.0; //게이지크기
int stage = 1; //게임 단계
int checkGoalValidation = 0; //골인지 아닌지 확인하는 변수
int bb = 0; //림의 이동을 제어해주는 변수
bool FirstRead = true;
bool startready = false;
bool opening = false; // true일때 라운드 시작
bool gamestart = false;
int Ready = 0;  // 라운드 시작 전 준비
GLuint TexId[22];  // 텍스처 객체

void Render();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y); 
void RenderFloor();
void Mouse(int button, int state, int x, int y);
void MouseMotion(int x, int y);

//재귀적으로 일정시간마다 호출된다.
void Timer(int id); 

void ThrowBall();
void BackBoard();
void InitOpenGL();
void ShowNumber(int n, int x, int y, int z);
void readycount();
int readycounttime=0;
unsigned char *ReadBmpFile(char *fname, int *w, int *h);

void score();
int ns = 0; // 슛한후 점수
int s = 0; // 그전의 점수

double time = 30.0; // 1라운드 제한시간

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(0, 0);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow("A SimpleGL Program");
	glutKeyboardFunc(Keyboard);

	void InitOpenGL();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(MouseMotion);
	glutTimerFunc(timerTerm, Timer, 1); //타이머 1시작

	glutMainLoop();
	return 0;
}

void InitOpenGL()
{
	// 깊이 버퍼 활성화
	glEnable(GL_DEPTH_TEST);

	// 조명 활성화
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat light_position0[4] = { 0.0f, 0.0f, 10.0f, 0.0f };
	GLfloat light_ambient0[] = { 1.0f, 1.0f, 1.0f };
	GLfloat light_diffuse0[] = { 1.0f, 1.0f, 1.0f };
	GLfloat light_specular0[] = { 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

	// 텍스춰 매핑 모드 설정
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	if (FirstRead)
	{
		//텍스처 아이디 생성
		glGenTextures(22, TexId); // 아이디 확보 TexId[0], TexId[1]에 저장...

								  // 텍스처 생성
		int w, h;
		unsigned char *pImage;
	
		pImage = ReadBmpFile("tex/0.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/1.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/2.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/3.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[3]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/4.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[4]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/5.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[5]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/6.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[6]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/7.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[7]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/8.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[8]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/9.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[9]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/11.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[10]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/22.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[11]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/33.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[12]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/ss.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[13]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/cr.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[14]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;
		
		pImage = ReadBmpFile("tex/f.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[15]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/title.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[16]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/end.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[17]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/r1.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[18]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/r2.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[19]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/r3.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[20]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		pImage = ReadBmpFile("tex/texture0.bmp", &w, &h);
		glBindTexture(GL_TEXTURE_2D, TexId[21]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pImage);
		delete[] pImage;

		FirstRead = false;
	}
}

/*
 * 이미지 파일을 읽어온다
 */
unsigned char *ReadBmpFile(char *fname, int *w, int *h)
{
	FILE *fp = fopen(fname, "rb");
	unsigned char *pData; 

	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	RGBTRIPLE rgb;

	// Read the fileheader
	fread(&fileheader, sizeof(fileheader), 1, fp);
	fseek(fp, sizeof(fileheader), SEEK_SET); // Jump the fileheader
	fread(&infoheader, sizeof(infoheader), 1, fp); // and read the infoheader

	*w = infoheader.biWidth;
	*h = infoheader.biHeight;
	int size = infoheader.biWidth * infoheader.biHeight;

	// Now we need to allocate the memory for our image (width * height * color deep)
	pData = new byte[size * 4];
	// And fill it with zeros

	memset(pData, 0, size * 4);

	// 픽셀 저장
	for (int i = 0, j = 0; i < size; ++i)
	{
		fread(&rgb, sizeof(rgb), 1, fp); // load RGB value
		pData[j + 0] = rgb.rgbtRed;
		pData[j + 1] = rgb.rgbtGreen;
		pData[j + 2] = rgb.rgbtBlue;
		pData[j + 3] = 255; // Alpha value
		j += 4;
	}
	fclose(fp);

	return pData;
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	Width = w;
	Height = h;
}


void SetupViewTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, Zoom);
	glRotatef(AngleX , 1.0f, 0.0f, 0.0f);
	glRotatef(AngleY , 0.0f, 1.0f, 0.0f);
}

void SetupViewVolume()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)Width/(double)Height, 1.0, 1000.0);
}

void Render()
{	
	

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetupViewTransform();
	SetupViewVolume();
	InitOpenGL();
	glMatrixMode(GL_MODELVIEW);

	

	// Set up material properties.
	GLfloat mat_ambient[] = { 0.1f, 0.1f, 0.1f, 0.0 };
	GLfloat mat_diffuse[] = { 0.7f, 0.7f, 0.7f, 0.0 };
	GLfloat mat_specular[] = { 0.9f, 0.9f, 0.9f, 0.8 };
	GLfloat mat_shininess = 10.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, &mat_shininess);

	glEnable(GL_TEXTURE_2D);

	//바닥을 그린다
	RenderFloor();

	if (!gamestart)
	{
		glBindTexture(GL_TEXTURE_2D, TexId[16]);

		glBegin(GL_POLYGON);
		{
			glTexCoord2d(0.0, 0.0);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(-10, -2, 0);

			glTexCoord2d(1.0, 0.0);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(10, -2, 0);

			glTexCoord2d(1.0, 1.0);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(10, 2, 0);

			glTexCoord2d(0.0, 1.0);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(-10, 2, 0);
		}
		glEnd();
	}

	

	glBindTexture(GL_TEXTURE_2D, TexId[21]);

	glBegin(GL_POLYGON);
	{
		glTexCoord2d(0.0, 0.0);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-10, -10, -20);

		glTexCoord2d(1.0, 0.0);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(10, -10, -20);

		glTexCoord2d(1.0, 1.0);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(10, 20, -20);

		glTexCoord2d(0.0, 1.0);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-10, 20, -20);
	}
	glEnd();

	if (!opening)
	{
		ShowNumber(0, 5, 6.5, -19);
		ShowNumber(0, 7, 6.5, -19);  //점수표시

		ShowNumber(0, -7, 6.5, -19);
		ShowNumber(3, -9, 6.5, -19);    //시간표시

		ShowNumber(0, -2, 12, -19);
		ShowNumber(stage, 0, 12, -19);  // 단계표시
	}

	readycount();
	
	
	if (opening)
	{
		if (stage == 1)
		{
			glBindTexture(GL_TEXTURE_2D, TexId[18]);

			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-7, 20, -20);

				glTexCoord2d(1.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(7, 20, -20);

				glTexCoord2d(1.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(7, 24, -20);

				glTexCoord2d(0.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-7, 24, -20);
			}
			glEnd();
		}

		if (stage == 2)
		{
			glBindTexture(GL_TEXTURE_2D, TexId[19]);

			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-7, 20, -20);

				glTexCoord2d(1.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(7, 20, -20);

				glTexCoord2d(1.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(7, 24, -20);

				glTexCoord2d(0.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-7, 24, -20);
			}
			glEnd();
		}

		if (stage == 3)
		{
			glBindTexture(GL_TEXTURE_2D, TexId[19]);

			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-7, 20, -20);

				glTexCoord2d(1.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(7, 20, -20);

				glTexCoord2d(1.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(7, 24, -20);

				glTexCoord2d(0.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-7, 24, -20);
			}
			glEnd();
		}

		BackBoard();  //백보드 그림

		glPushMatrix();
		glTranslatef(0, -4, 0);
		glRotatef(180, 0, 1, 0);
		glRotatef(-30, 1, 0, 0);
		glRotatef(-tempro - AngleY, 0, 1, 0);
		glutSolidCone(0.5, 2.5, 10, 10);
		glPopMatrix();//방향 화살표

		glPushMatrix();
		glTranslatef(0, -4.75, 5);
		glScaled(a, 1, 1);
		glutSolidCube(0.5);
		glPopMatrix();
		//게이지

		ThrowBall();

		
		ShowNumber(ns / 10, 5, 6.5, -19);
		ShowNumber((ns % 10), 7, 6.5, -19);  //점수표시

		ShowNumber(((int)time % 10), -7, 6.5, -19);
		ShowNumber(((int)(time/10) % 10), -9, 6.5, -19);    //시간표시

		ShowNumber(0, -2, 12, -19);
		ShowNumber(stage, 0, 12, -19);  // 단계표시
	}

	glutSwapBuffers();
	
}

void readycount()
{
	if (startready)
	{
		if (readycounttime > 1 && readycounttime <100)
		{
			glBindTexture(GL_TEXTURE_2D, TexId[10]);

			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-2, 20, -20);

				glTexCoord2d(1.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(2, 20, -20);

				glTexCoord2d(1.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(2, 24, -20);

				glTexCoord2d(0.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-2, 24, -20);
			}
			glEnd();
		}

		if (readycounttime >101 && readycounttime < 200)
		{
			glBindTexture(GL_TEXTURE_2D, TexId[11]);

			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-2, 20, -20);

				glTexCoord2d(1.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(2, 20, -20);

				glTexCoord2d(1.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(2, 24, -20);

				glTexCoord2d(0.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-2, 24, -20);
			}
			glEnd();
		}

		if (readycounttime >201 && readycounttime < 300)
		{
			glBindTexture(GL_TEXTURE_2D, TexId[12]);

			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-2, 20, -20);

				glTexCoord2d(1.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(2, 20, -20);

				glTexCoord2d(1.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(2, 24, -20);

				glTexCoord2d(0.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-2, 24, -20);
			}
			glEnd();
		}

		if (readycounttime >301 && readycounttime <= 400)
		{

			glBindTexture(GL_TEXTURE_2D, TexId[13]);

			glBegin(GL_POLYGON);
			{
				glTexCoord2d(0.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-7, 20, -20);

				glTexCoord2d(1.0, 0.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(7, 20, -20);

				glTexCoord2d(1.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(7, 24, -20);

				glTexCoord2d(0.0, 1.0);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3f(-7, 24, -20);
			}
			glEnd();

			if (readycounttime == 400)
			{
				readycounttime = 0;
				opening = true;
				startready = false;
			}
			
		}
	}
}

void BackBoard()
{
	rx = wx;
	ry = wy - 2;
	rz = wz + 5;

	if (stage == 1)//1단계 기본
	{
	}

	else if (stage == 2) //2단계 좌우로 움직임
	{
			if (bb == 0 )
				wx = wx + 0.05;

			if (wx >= 6.5)
				bb = 1;

			if (bb == 1)
				wx = wx - 0.05;

			if (wx <= -6.5)
				bb = 0;
	}
	
	else if (stage == 3)//3단계
	{
		if (bb == 0 )
			wx = wx + 0.1;

		if (wx >= 6.5)
			bb = 1;

		if (bb == 1)
			wx = wx - 0.1;

		if (wx <= -6.5)
			bb = 0;
	}

	//glPushMatrix();
	//glTranslatef(wx, wy, wz);
	//glScaled(1.0, 0.8, 0.1);
	//glColor3f(0, 0, 0.7);
	//glutSolidCube(25);
	//glPopMatrix();// 백보드

	glPushMatrix();
	glTranslatef(rx, ry, rz);
	glRotatef(90, 1, 0, 0);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidTorus(0.5, 4, 20, 20);
	glPopMatrix();// 림
}

void ThrowBall()
{

	if (tt != 0) //공이 날아가는 중이라면
	{

		if (bz < wz + 2 && bz > wz && vz < 0
			&& bx < wx + 10 && bx > wx - 10
			&& by < wy + 8 && by > wy - 8)         //백보드 충돌
			vz = vz * -0.5;

		else if (bz < rz + 6 && bz > rz + 4 && vz < 0
			&& bx < rx + 4 && bx > rx - 4
			&& by < ry + 1.5 && by > ry - 1.5)         // 림front-out 충돌
			vz = vz * -0.7;

		else if (bz < rz + 4 && bz > rz + 1.5 && vz > 0
			&& bx < rx + 4 && bx > rx - 4
			&& by < ry + 1.5 && by > ry - 1.5)         // 림front-in 충돌
		{
			vz *= -0.4;
			vy *= 0.5;
			checkGoalValidation = 1;
		}

		else if (bx < rx - 4 && bx > rx - 6 && vx > 0
			&& bz < rz + 4 && bz > rz - 4
			&& by < ry + 1.5 && by > ry - 1.5)         // 림left-out 충돌
			vx = vx * -0.7;

		else if (bx < rx - 1.5 && bx > rx - 4 && vx < 0
			&& bz < rz + 4 && bz > rz - 4
			&& by < ry + 1.5 && by > ry - 1.5)         // 림left-in 충돌
		{
			vx *= -0.4;
			vy *= 0.5;
			checkGoalValidation = 1;
		}
		else if (bx < rx + 6 && bx > rx + 4 && vx > 0
			&& bz < rz + 4 && bz > rz - 4
			&& by < ry + 1.5 && by > ry - 1.5)         // 림right-in 충돌
		{
			vx *= -0.4;
			vy *= 0.5;
			checkGoalValidation = 1;
		}
		else if (bx < rx + 4 && bx > rx + 1.5 && vx < 0
			&& bz < rz + 4 && bz > rz - 4
			&& by < ry + 1.5 && by > ry - 1.5)         // 림right-out 충돌
			vx = vx * -0.7;

		
		glPushMatrix();

		bx += vx * 0.012;

		bz += vz * 0.012;

		//공의 z좌표

		vy += -100 * 0.01;  //공의 y좌표

		by += vy * 0.012;

		glTranslatef(bx, by, bz);


		glColor3f(0.8, 0.5, 0);
		glutSolidSphere(2.5, 50, 20); //좌표에 맞게 공을 그린다

		glPopMatrix();// 공

		score();
		if (checkGoalValidation == 1)
		{
			ns = s + 2;
		}

		//printf("%d\n", ns);
	}

	if (by <= -5)
	{
		fire = 0;
		tt = 0;

		bx = 0;
		by = 0;
		bz = 0;
	}
}

void ShowNumber(int n, int x, int y, int z)
{
	glPushMatrix();
	glTranslatef(x, y, z);

	glBindTexture(GL_TEXTURE_2D, TexId[n]);
	glBegin(GL_POLYGON);
	{
		glTexCoord2d(0.0, 0.0);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(0, 0, 0);

		glTexCoord2d(1.0, 0.0);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(2, 0, 0);

		glTexCoord2d(1.0, 1.0);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(2, 4, 0);

		glTexCoord2d(0.0, 1.0);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(0, 4, 0);
	}
	glEnd();
	glPopMatrix();
}


void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'a') AngleY -= 3.0;
	if (key == 's') AngleY += 3.0;
	if (key == 'd') AngleX -= 3.0;
	if (key == 'f') AngleX += 3.0;
	if (key == 'w') Zoom -= 2.0;
	if (key == 'x') Zoom += 2.0;
	if (key == 'q') ro -= 4.0;
	if (key == 'e') ro += 4.0;
	if (key == 't')
	{
		gamestart = true;
		startready = true;
		wx = 0;
		wy = 10;
		wz = -20;
	}

	glutPostRedisplay();
}

/*
 * 마우스 움직임에 따라 공의 발사 각도를 조절한다
 */
void MouseMotion(int x, int y)
{	
	tempro = 180/pi*atan(double(x - 500) / double(500 - y));
} 

void Mouse(int button, int state, int x, int y)
{
	if (button == 0 && state == 0)   //마우스를 누르면
	{
		checkGoalValidation = 0;
		s = ns;
		ms = 0;
		st = t;
	}
	else if (button == 0 && state == 1)  //마우스를 떼면
	{
		ro = tempro + AngleY;
		ms = 1;
		et = t;
		v = et - st;
		
		a = 0.0;
		
		if (fire == 0)
		{
			double piro = ro / 180 * pi;  //각도를 라디안으로 변환

			vx = sin(piro)*v;
			vy = 0.8 * v * 2;
			vz = cos(piro)*-v;   //각도에 맞춰 공의 속도 조정

			//printf("%d,%d,%d,%d\n", vx, vy, vz,v);

			
		}

		fire = 1;
	}

	else if (button == 1 && state == 0)  //휠
	{
		Zoom += 2;
	}

	else if (button == 2 && state == 0)  //휠
	{
		Zoom -= 2;
	}
}

/*
 * 격자 문양의 바닥을 그린다 
 */
void RenderFloor()
{
	glColor3f(0, 0, 0);
	for (float t = -50; t <= 50; t += 1.0)
	{
		glBegin(GL_LINES);
		glVertex3f(t, -10, -50);
		glVertex3f(t, -10, 50);
		glVertex3f(-50, -10, t);
		glVertex3f(50, -10, t);
		glEnd();
	}
}

void Timer(int id)
{
		if (id == 1)
		{
			if (startready)
				readycounttime++;
			if (ms == 0)  //마우스가 눌려있으면 a를 증가
				a += 1;
			if (fire == 1)  //발사 중이면 tt를 증가
				tt += 0.5;
			t += 1;

			if (opening == true)
			time = time - 0.01;

			if (time <= 0)
			{
				opening = false;
				time = 30.0;
				if (ns >= 12)
					stage++;
				ns = 0;
				if (stage == 4)
				{
					opening = false;
					stage = 1;
				}
					
			}

			glutPostRedisplay();
			glutTimerFunc(timerTerm, Timer, 1);
		}
}

void score()
{
	if (vy < 0)
	{
		if (bx < rx + 2.5 && rx - 2.5 < bx)
		{
			if (by < ry + 0.2 && ry - 0.2 < by)
			{
				if (bz < rz + 2.5 && rz - 2.5 < bz)
					checkGoalValidation = 1;
			}
		}
	}
}
