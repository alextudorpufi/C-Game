/// SEGMENTE - TUDOR ALEXANDRU, URSACHE STEFAN

// -- LIBRARY - SECTION -------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <graphics.h>
#include <iostream>
#include <dos.h>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <cmath>

using namespace std;

// -- DATA - SECTION --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ESC 27
int nrNoduri, nrMuchii, k, loadingVal=0, MENTOSAN=0, BOMBOCLAT=0;
int lives1=3, lives2=3;
int graph[1000][1000];
char tasta;
bool player; //0 - player 1 / 1- player 2
bool usedRandomMove = false;
bool usedRandomMovePlayer1 = false;
bool usedRandomMovePlayer2 = false;
bool noPossibleMove = false;
bool skipStepOne = false;
bool comingFromPVC = false;
bool amGasit = false;
bool language = 0;
int defaultBackgroundColor, defaultDotsColor, defaultPlayer1Color, defaultPlayer2Color;
int backgroundColor, dotsColor, player1Color, player2Color;
struct nod
{
    int numar;
    int x,y;
} listaNoduri[21];

struct segment
{
    nod nod1, nod2;
} v[1000];
int FirstNod, SecondNod;

// -- FUNCTIONS - HEADER - SECTION -----------------------------------------------------------------------------------------------------------------------------------------------

void cleanup();
void randomMove();
void trasareSegment(int coord1,int coord2);
void displayError(nod a, nod b);
void loadingScreen();
void button(int &x1, int &y1, int &x2, int &y2, int width, int height, char *text);
void menu();
void afisteme();
void showLivesPVC();
void selectlevel();
void afistutorial();
void rectangleInfo();
void deseneazaNod(int x, int y, int i);
void draw20RandomDots();
void getDots();
void blankError();
void randomDots();
void afislimba();
void showLives();
void joc();
void surrender();
void sfarsit();
void help();
void jocPVC();
bool SePoate(int nod1, int nod2);
bool SePoateRandom(int nod1, int nod2);
int det(int X1, int Y1, int X2, int Y2, int X3, int Y3);
int pctsegm(int x1, int y1, int x2, int y2, int x3, int y3);
int waitFirstNod();
int waitSecondNod();

// -- MAIN - SECTION -------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{

    initwindow(800,800, "Segmente");

    defaultBackgroundColor = 3;
    backgroundColor = defaultBackgroundColor;
    defaultDotsColor = 4;
    dotsColor = defaultDotsColor;
    defaultPlayer1Color = 15;
    player1Color = defaultPlayer1Color;
    defaultPlayer2Color = 0;
    player2Color = defaultPlayer2Color;
    menu();

    getch();
    closegraph();
    system("PAUSE"); // Altfel nu merge
    return 0;

}

// -- FUNCTIONS - CONTENT - SECTION -------------------------------------------------------------------------------------------------------------------------------------------

void surrender()   /// Frontend function ("Ma predau!")
{

    // <- initialization ->
    settextstyle(4, 0, 1);
    setcolor(MAGENTA);
    int surrenderX1 = 100, surrenderY1 = 765, surrenderX2, surrenderY2;

    // <- button ->

    if(language == 0)
        button(surrenderX1, surrenderY1, surrenderX2, surrenderY2, 50, 20, "Ma predau!");
    else
        button(surrenderX1, surrenderY1, surrenderX2, surrenderY2, 50, 20, "Surrender!");

    // <- box ->

    setcolor(BLACK);
    rectangle(50, 765, 200, 785);
    rectangle(48, 763, 202, 787);
    line(195, 785, 200, 780);
    line(193, 785, 200, 778);
    line(55, 765, 50, 770);
    line(57, 765, 50, 772);

}

void help()   /// Frontend function ("Help?!")
{

    // <- initialization ->

    settextstyle(4, 0, 1);
    setcolor(MAGENTA);
    int helpX1 = 675, helpY1 = 765, helpX2, helpY2;

    // <- button ->

    if(language == 0)
        button(helpX1, helpY1, helpX2, helpY2, 50, 20, "Ajutor?!");
    else
        button(helpX1, helpY1, helpX2, helpY2, 50, 20, "Help?!");

    // <- box ->

    setcolor(BLACK);
    rectangle(645, 765, 755, 785);
    rectangle(643, 763, 757, 787);
    line(750, 765, 755, 770);
    line(748, 765, 755, 772);
    line(645, 780, 650, 785);
    line(645, 778, 652, 785);

}

void loadingScreen() /// Frontend loading screen
{

    // <- initialization ->

    loadingVal = 1;
    readimagefile("logoblack.jpg", 210, 90, 590, 185);
    rectangle(325,390,475,410);
    setcolor(CYAN);
    outtextxy(360, 415, "se incarca...");
    setcolor(LIGHTGRAY);
    outtextxy(230, 770, "Proiect realizat de: Tudor Alexandru, Ursache Stefan");
    setcolor(CYAN);

    // <- animation ->

    for(int i=0; i<=146; i++)
    {
        line(327+i,392,327+i,408);
        delay(15);
    }

    // <- next step ->

    cleardevice();
    menu();

}

void joc() /// Game Main
{

    // <- loop ->

    while(lives1!=0 and lives2!=0)
    {
        usedRandomMove = false;
        amGasit = false;

        outtextxy(5,60,"                                                                                                                        ");
        surrender();
        help();
        showLives();
        if(skipStepOne == false) waitFirstNod();
        else skipStepOne = false;
        if(usedRandomMove == true and player == 0)
            usedRandomMovePlayer1 = true;
        else if(usedRandomMove == true and player == 1)
            usedRandomMovePlayer2 = true;
        if(usedRandomMove == false)
        {
            waitSecondNod();
            SePoate(FirstNod, SecondNod);
        }
        player = 1-player;
    }

    // <- next step ->

    sfarsit();

}

void jocPVC() /// Game Main
{
    // <- loop ->

    while(lives1!=0)
    {
        usedRandomMove = false;
        amGasit = false;

        outtextxy(5,60,"                                                                                                                        ");
        surrender();
        help();
        showLivesPVC();
        if(skipStepOne == false) waitFirstNod();
        else skipStepOne = false;
        if(usedRandomMove == true and player == 0)
            usedRandomMovePlayer1 = true;
        if(usedRandomMove == false)
        {
            waitSecondNod();
            SePoate(FirstNod, SecondNod);
        }

        player = 1 - player;

        amGasit = false;
        randomMove();
        if(noPossibleMove == true)
            sfarsit();

        player = 1 - player;
    }


    // <- next step ->

    sfarsit();

}

int waitFirstNod() /// Wait for first point selection
{

    // <- info ->

    outtextxy(170,60,"                                                                                                                                                   ");

    if(language == 0)
        outtextxy(220,60,"Te rog selecteaza primul punct.");
    else
        outtextxy(240,60,"Please select the first point.");
    // <- wait click ->

    int mouseX = 0;
    int mouseY = 0;
    skipStepOne = false;

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            mouseX = mousex();
            mouseY = mousey();

            for(int i=1; i<=20; i++)
                if (mouseX >=listaNoduri[i].x-3 && mouseX <= listaNoduri[i].x+3 &&
                        mouseY >= listaNoduri[i].y-3 && mouseY <= listaNoduri[i].y+3)
                {
                    FirstNod=i;
                    return 0;
                }
                else if (mouseX >=48 && mouseX <= 202 &&
                         mouseY >= 764 && mouseY <= 787)
                {
                    if(comingFromPVC == false)  player = 1 - player;
                    sfarsit();
                }
                else if (mouseX >= 643 && mouseX <= 757 &&
                         mouseY >= 763 && mouseY <= 787)
                {
                    if(player == 0 and usedRandomMovePlayer1 == false or
                            player == 1 and usedRandomMovePlayer2 == false)
                    {
                        randomMove();
                        if(noPossibleMove == true)
                        {
                            outtextxy(120,60,"                                                                                                                                       ");

                            if(language == 0)
                                outtextxy(120,60,"Buton inactiv! Nu mai poti trasa niciun segment.");
                            else
                                outtextxy(120,60,"Button disabled! No possible options to draw.");

                            delay(2000);
                            if(comingFromPVC == false)
                                joc();
                            else
                                jocPVC();
                        }
                        else
                        {
                            usedRandomMove = true;
                            return 0;
                        }
                    }
                    else
                    {
                        outtextxy(170,60,"                                                                                                                                       ");

                        if(language == 0)
                            outtextxy(170,60,"Buton inactiv! Ai folosit deja ajutorul.");
                        else
                            outtextxy(170,60,"Button disabled! You already used it.");

                        delay(2000);
                        if(comingFromPVC == false)
                            joc();
                        else
                            jocPVC();
                    }
                }
        }
    }

}

int waitSecondNod() /// Wait for second point selection !!!!!!!!!!!!!!!!!!!! BUGUITA NU STIU CE ARE
{

    // <- info ->

    outtextxy(170,60,"                                                                                                                                                       ");

    if(language == 0)
        outtextxy(220,60,"Te rog selecteaza al doilea punct.");
    else
        outtextxy(220,60,"Please select the second point.");

    // <- wait click ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();
            for(int i=1; i<=20; i++)
                if (mouseX >=listaNoduri[i].x-6 && mouseX <= listaNoduri[i].x+6 &&
                        mouseY >= listaNoduri[i].y-6 && mouseY <= listaNoduri[i].y+6)
                {
                    SecondNod=i;
                    return 0;
                }
                else if (mouseX >=48 && mouseX <= 202 &&
                         mouseY >= 764 && mouseY <= 787)
                    sfarsit();
                else if (mouseX >= 643 && mouseX <= 757 &&
                         mouseY >= 763 && mouseY <= 787)
                {
                    outtextxy(170,60,"                                                                                                                                       ");

                    if(language == 0)
                        outtextxy(185,60,"Buton inactiv! Ai selectat primul punct.");
                    else
                        outtextxy(170,60,"Button disabled! You selected the first point.");


                    delay(2000);
                    skipStepOne = true;
                    if(comingFromPVC == false)
                        joc();
                    else
                        jocPVC();
                }
        }
    }

}

void afisteme()
{
    // <- initialization ->

    setbkcolor(3);
    cleardevice();
    readimagefile("logoblue.jpg", 210, 90, 590, 185);
    settextstyle(4, 0, 2);

    // <- info ->

    if(language == 0)
    {
        outtextxy(240, 210, "Selecteaza culorile dorite");
        outtextxy(190, 230, "daca doresti sa modifici o alegere");
        outtextxy(165, 250, "finalizeaza selectia si revino in meniu.");


        setcolor(RED);
        outtextxy(335, 300, "!!Atentie!!");
        setcolor(WHITE);
        outtextxy(105, 320, "Selecteaza culori ce nu afecteaza vizibilitatea.");
    }
    else
    {
        outtextxy(230, 210, "Select the desired colors");
        outtextxy(180, 230, "if you want to modify the selection");
        outtextxy(105, 250, "complete the selection and return to the menu.");

        setcolor(RED);
        outtextxy(335, 300, "!!Caution!!");
        setcolor(WHITE);
        outtextxy(135, 320, "Select colors that do not affect visibility.");

    }

    // <- background color ->

    // <- info ->

    outtextxy(240, 370, "                                                                                                                               ");

    if(language == 0)
        outtextxy(270, 370, "Culoarea fundalului:");
    else
        outtextxy(280, 370, "Background color:");

    // <- buttons ->

    setcolor(WHITE);

    setfillstyle(SOLID_FILL, BLUE);
    bar(225, 420, 275, 470);
    rectangle(225, 420, 275, 470);

    setfillstyle(SOLID_FILL, CYAN);
    bar(325, 420, 375, 470);
    rectangle(325, 420, 375, 470);

    setfillstyle(SOLID_FILL, BROWN);
    bar(425, 420, 475, 470);
    rectangle(425, 420, 475, 470);

    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(525, 420, 575, 470);
    rectangle(525, 420, 575, 470);

    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(225, 520, 275, 570);
    rectangle(225, 520, 275, 570);

    setfillstyle(SOLID_FILL, GREEN);
    bar(325, 520, 375, 570);
    rectangle(325, 520, 375, 570);

    setfillstyle(SOLID_FILL, LIGHTMAGENTA);
    bar(425, 520, 475, 570);
    rectangle(425, 520, 475, 570);

    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(525, 520, 575, 570);
    rectangle(525, 520, 575, 570);

    // <- wait selection ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();

            if (mouseX >= 225 && mouseX <= 275 &&
                    mouseY >= 420 && mouseY <= 470)   /// BLUE
            {

                setcolor(YELLOW);
                rectangle(225, 420, 275, 470);
                rectangle(226, 421, 274, 469);
                backgroundColor = 1;
                break;

            }
            else if (mouseX >= 325 && mouseX <= 375 &&
                     mouseY >= 420 && mouseY <= 470)   /// CYAN
            {

                setcolor(YELLOW);
                rectangle(325, 420, 375, 470);
                rectangle(326, 421, 374, 469);
                backgroundColor = 3;
                break;

            }
            else if (mouseX >= 425 && mouseX <= 475 &&
                     mouseY >= 420 && mouseY <= 470)   /// BROWN
            {

                setcolor(YELLOW);
                rectangle(425, 420, 475, 470);
                rectangle(426, 421, 474, 469);
                backgroundColor = 6;
                break;

            }
            else if (mouseX >= 525 && mouseX <= 575 &&
                     mouseY >= 420 && mouseY <= 470)   /// LIGHTBLUE
            {

                setcolor(YELLOW);
                rectangle(525, 420, 575, 470);
                rectangle(526, 421, 574, 469);
                backgroundColor = 9;
                break;

            }
            else if (mouseX >= 225 && mouseX <= 275 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTGREEN
            {

                setcolor(YELLOW);
                rectangle(225, 520, 275, 570);
                rectangle(226, 521, 274, 569);
                backgroundColor = 10;
                break;

            }
            else if (mouseX >= 325 && mouseX <= 375 &&
                     mouseY >= 520 && mouseY <= 570)   /// GREEN
            {

                setcolor(YELLOW);
                rectangle(325, 520, 375, 570);
                rectangle(326, 521, 374, 569);
                backgroundColor = 2;
                break;

            }
            else if (mouseX >= 425 && mouseX <= 475 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTMAGENTA
            {

                setcolor(YELLOW);
                rectangle(425, 520, 475, 570);
                rectangle(426, 521, 474, 569);
                backgroundColor = 13;
                break;

            }
            else if (mouseX >= 525 && mouseX <= 575 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTCYAN
            {

                setcolor(YELLOW);
                rectangle(525, 520, 575, 570);
                rectangle(526, 521, 574, 569);
                backgroundColor = 11;
                break;

            }
        }
    }

    // <- initialization ->

    settextstyle(4, 0, 3);
    setcolor(YELLOW);

    // <- button ->

    int nextX1 = 370, nextY1 = 700, nextX2, nextY2;

    if(language == 1)
    {
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "Next");
        setcolor(BLACK);
        rectangle(335, 695, 450, 725);

        nextX1 = 335;
        nextX2 = 450;
        nextY1 = 695;
        nextY2 = 725;
    }
    else
    {
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "Urmatorul");
        setcolor(BLACK);
        rectangle(305, 695, 480, 725);

        nextX1 = 305;
        nextX2 = 480;
        nextY1 = 695;
        nextY2 = 725;
    }



    // <- wait click ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();
            if (mouseX >=nextX1-3 && mouseX <= nextX2+3 &&
                    mouseY >= nextY1-3 && mouseY <= nextY2+3)
                break;
        }
    }


    if(language == 0)
    {

        setcolor(CYAN);
        rectangle(305, 695, 480, 725);
        nextX1 = 305;
        nextX2 = 480;
        nextY1 = 695;
        nextY2 = 725;
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "                                  ");
    }
    else
    {
        setcolor(CYAN);
        rectangle(335, 695, 450, 725);
        nextX1 = 335;
        nextX2 = 450;
        nextY1 = 695;
        nextY2 = 725;
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "                                  ");
    }

// <- dots color ->

// <- info ->

    setcolor(WHITE);
    outtextxy(240, 370, "                                                                                                                               ");

    if(language == 0)
        outtextxy(270, 370, "Culoarea punctelor:");
    else
        outtextxy(300, 370, "Dots color:");

// <- buttons ->

    setcolor(WHITE);

    setfillstyle(SOLID_FILL, BLUE);
    bar(225, 420, 275, 470);
    rectangle(225, 420, 275, 470);

    setfillstyle(SOLID_FILL, CYAN);
    bar(325, 420, 375, 470);
    rectangle(325, 420, 375, 470);

    setfillstyle(SOLID_FILL, BROWN);
    bar(425, 420, 475, 470);
    rectangle(425, 420, 475, 470);

    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(525, 420, 575, 470);
    rectangle(525, 420, 575, 470);

    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(225, 520, 275, 570);
    rectangle(225, 520, 275, 570);

    setfillstyle(SOLID_FILL, GREEN);
    bar(325, 520, 375, 570);
    rectangle(325, 520, 375, 570);

    setfillstyle(SOLID_FILL, LIGHTMAGENTA);
    bar(425, 520, 475, 570);
    rectangle(425, 520, 475, 570);

    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(525, 520, 575, 570);
    rectangle(525, 520, 575, 570);


// <- wait selection ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();

            if (mouseX >= 225 && mouseX <= 275 &&
                    mouseY >= 420 && mouseY <= 470)   /// BLUE
            {

                setcolor(YELLOW);
                rectangle(225, 420, 275, 470);
                rectangle(226, 421, 274, 469);
                dotsColor = 1;
                break;

            }
            else if (mouseX >= 325 && mouseX <= 375 &&
                     mouseY >= 420 && mouseY <= 470)   /// CYAN
            {

                setcolor(YELLOW);
                rectangle(325, 420, 375, 470);
                rectangle(326, 421, 374, 469);
                dotsColor = 3;
                break;

            }
            else if (mouseX >= 425 && mouseX <= 475 &&
                     mouseY >= 420 && mouseY <= 470)   /// BROWN
            {

                setcolor(YELLOW);
                rectangle(425, 420, 475, 470);
                rectangle(426, 421, 474, 469);
                dotsColor = 6;
                break;

            }
            else if (mouseX >= 525 && mouseX <= 575 &&
                     mouseY >= 420 && mouseY <= 470)   /// LIGHTBLUE
            {

                setcolor(YELLOW);
                rectangle(525, 420, 575, 470);
                rectangle(526, 421, 574, 469);
                dotsColor = 9;
                break;

            }
            else if (mouseX >= 225 && mouseX <= 275 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTGREEN
            {

                setcolor(YELLOW);
                rectangle(225, 520, 275, 570);
                rectangle(226, 521, 274, 569);
                dotsColor = 10;
                break;

            }
            else if (mouseX >= 325 && mouseX <= 375 &&
                     mouseY >= 520 && mouseY <= 570)   /// GREEN
            {

                setcolor(YELLOW);
                rectangle(325, 520, 375, 570);
                rectangle(326, 521, 374, 569);
                dotsColor = 2;
                break;

            }
            else if (mouseX >= 425 && mouseX <= 475 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTMAGENTA
            {

                setcolor(YELLOW);
                rectangle(425, 520, 475, 570);
                rectangle(426, 521, 474, 569);
                dotsColor = 13;
                break;

            }
            else if (mouseX >= 525 && mouseX <= 575 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTCYAN
            {

                setcolor(YELLOW);
                rectangle(525, 520, 575, 570);
                rectangle(526, 521, 574, 569);
                dotsColor = 11;
                break;

            }
        }
    }

// <- initialization ->

    settextstyle(4, 0, 3);
    setcolor(YELLOW);

// <- button ->

    nextX1 = 370, nextY1 = 700, nextX2, nextY2;
    if(language == 1)
    {
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "Next");
        setcolor(BLACK);
        rectangle(335, 695, 450, 725);

        nextX1 = 335;
        nextX2 = 450;
        nextY1 = 695;
        nextY2 = 725;
    }
    else
    {
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "Urmatorul");
        setcolor(BLACK);
        rectangle(305, 695, 480, 725);

        nextX1 = 305;
        nextX2 = 480;
        nextY1 = 695;
        nextY2 = 725;
    }

// <- wait click ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();
            if (mouseX >=nextX1-3 && mouseX <= nextX2+3 &&
                    mouseY >= nextY1-3 && mouseY <= nextY2+3)
                break;
        }
    }

    if(language == 0)
    {
        setcolor(CYAN);
        rectangle(305, 695, 480, 725);
        nextX1 = 305;
        nextX2 = 480;
        nextY1 = 695;
        nextY2 = 725;
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "                                  ");
    }
    else
    {
        setcolor(CYAN);
        rectangle(335, 695, 450, 725);
        nextX1 = 335;
        nextX2 = 450;
        nextY1 = 695;
        nextY2 = 725;
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "                                  ");
    }

// <- player 1 color ->

// <- info ->

    setcolor(WHITE);
    outtextxy(240, 370, "                                                                                                                               ");

    if(language == 0)
        outtextxy(270, 370, "Culoare jucator 1:");
    else
        outtextxy(280, 370, "Player 1 color:");

// <- buttons ->

    setcolor(WHITE);

    setfillstyle(SOLID_FILL, BLUE);
    bar(225, 420, 275, 470);
    rectangle(225, 420, 275, 470);

    setfillstyle(SOLID_FILL, CYAN);
    bar(325, 420, 375, 470);
    rectangle(325, 420, 375, 470);

    setfillstyle(SOLID_FILL, BROWN);
    bar(425, 420, 475, 470);
    rectangle(425, 420, 475, 470);

    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(525, 420, 575, 470);
    rectangle(525, 420, 575, 470);

    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(225, 520, 275, 570);
    rectangle(225, 520, 275, 570);

    setfillstyle(SOLID_FILL, GREEN);
    bar(325, 520, 375, 570);
    rectangle(325, 520, 375, 570);

    setfillstyle(SOLID_FILL, LIGHTMAGENTA);
    bar(425, 520, 475, 570);
    rectangle(425, 520, 475, 570);

    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(525, 520, 575, 570);
    rectangle(525, 520, 575, 570);


// <- wait selection ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();

            if (mouseX >= 225 && mouseX <= 275 &&
                    mouseY >= 420 && mouseY <= 470)   /// BLUE
            {

                setcolor(YELLOW);
                rectangle(225, 420, 275, 470);
                rectangle(226, 421, 274, 469);
                player1Color = 1;
                break;

            }
            else if (mouseX >= 325 && mouseX <= 375 &&
                     mouseY >= 420 && mouseY <= 470)   /// CYAN
            {

                setcolor(YELLOW);
                rectangle(325, 420, 375, 470);
                rectangle(326, 421, 374, 469);
                player1Color = 3;
                break;

            }
            else if (mouseX >= 425 && mouseX <= 475 &&
                     mouseY >= 420 && mouseY <= 470)   /// BROWN
            {

                setcolor(YELLOW);
                rectangle(425, 420, 475, 470);
                rectangle(426, 421, 474, 469);
                player1Color = 6;
                break;

            }
            else if (mouseX >= 525 && mouseX <= 575 &&
                     mouseY >= 420 && mouseY <= 470)   /// LIGHTBLUE
            {

                setcolor(YELLOW);
                rectangle(525, 420, 575, 470);
                rectangle(526, 421, 574, 469);
                player1Color = 9;
                break;

            }
            else if (mouseX >= 225 && mouseX <= 275 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTGREEN
            {

                setcolor(YELLOW);
                rectangle(225, 520, 275, 570);
                rectangle(226, 521, 274, 569);
                player1Color = 10;
                break;

            }
            else if (mouseX >= 325 && mouseX <= 375 &&
                     mouseY >= 520 && mouseY <= 570)   /// GREEN
            {

                setcolor(YELLOW);
                rectangle(325, 520, 375, 570);
                rectangle(326, 521, 374, 569);
                player1Color = 2;
                break;

            }
            else if (mouseX >= 425 && mouseX <= 475 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTMAGENTA
            {

                setcolor(YELLOW);
                rectangle(425, 520, 475, 570);
                rectangle(426, 521, 474, 569);
                player1Color = 13;
                break;

            }
            else if (mouseX >= 525 && mouseX <= 575 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTCYAN
            {

                setcolor(YELLOW);
                rectangle(526, 521, 574, 569);
                player1Color = 11;
                break;

            }
        }
    }

// <- initialization ->

    settextstyle(4, 0, 3);
    setcolor(YELLOW);

// <- button ->

    nextX1 = 370, nextY1 = 700, nextX2, nextY2;
    if(language == 1)
    {
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "Next");
        setcolor(BLACK);
        rectangle(335, 695, 450, 725);

        nextX1 = 335;
        nextX2 = 450;
        nextY1 = 695;
        nextY2 = 725;
    }
    else
    {
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "Urmatorul");
        setcolor(BLACK);
        rectangle(305, 695, 480, 725);

        nextX1 = 305;
        nextX2 = 480;
        nextY1 = 695;
        nextY2 = 725;
    }

// <- wait click ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();
            if (mouseX >=nextX1-3 && mouseX <= nextX2+3 &&
                    mouseY >= nextY1-3 && mouseY <= nextY2+3)
                break;
        }
    }

// <- player 2 color ->

// <- info ->

    if(language == 0)
    {
        setcolor(CYAN);
        rectangle(305, 695, 480, 725);
        nextX1 = 305;
        nextX2 = 480;
        nextY1 = 695;
        nextY2 = 725;
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "                                  ");
    }
    else
    {
        setcolor(CYAN);
        rectangle(335, 695, 450, 725);
        nextX1 = 335;
        nextX2 = 450;
        nextY1 = 695;
        nextY2 = 725;
        button(nextX1, nextY1, nextX2, nextY2, 50, 20, "                                  ");
    }

    setcolor(WHITE);
    outtextxy(240, 370, "                                                                                                                               ");

    if(language == 0)
        outtextxy(270, 370, "Culoare jucator 2:");
    else
        outtextxy(280, 370, "Player 2 color:");

// <- buttons ->

    setcolor(WHITE);

    setfillstyle(SOLID_FILL, BLUE);
    bar(225, 420, 275, 470);
    rectangle(225, 420, 275, 470);

    setfillstyle(SOLID_FILL, CYAN);
    bar(325, 420, 375, 470);
    rectangle(325, 420, 375, 470);

    setfillstyle(SOLID_FILL, BROWN);
    bar(425, 420, 475, 470);
    rectangle(425, 420, 475, 470);

    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(525, 420, 575, 470);
    rectangle(525, 420, 575, 470);

    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(225, 520, 275, 570);
    rectangle(225, 520, 275, 570);

    setfillstyle(SOLID_FILL, GREEN);
    bar(325, 520, 375, 570);
    rectangle(325, 520, 375, 570);

    setfillstyle(SOLID_FILL, LIGHTMAGENTA);
    bar(425, 520, 475, 570);
    rectangle(425, 520, 475, 570);

    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(525, 520, 575, 570);
    rectangle(525, 520, 575, 570);


// <- wait selection ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();

            if (mouseX >= 225 && mouseX <= 275 &&
                    mouseY >= 420 && mouseY <= 470)   /// BLUE
            {

                setcolor(YELLOW);
                rectangle(225, 420, 275, 470);
                rectangle(226, 421, 274, 469);
                player2Color = 1;
                break;

            }
            else if (mouseX >= 325 && mouseX <= 375 &&
                     mouseY >= 420 && mouseY <= 470)   /// CYAN
            {

                setcolor(YELLOW);
                rectangle(325, 420, 375, 470);
                rectangle(326, 421, 374, 469);
                player2Color = 3;
                break;

            }
            else if (mouseX >= 425 && mouseX <= 475 &&
                     mouseY >= 420 && mouseY <= 470)   /// BROWN
            {

                setcolor(YELLOW);
                rectangle(425, 420, 475, 470);
                rectangle(426, 421, 474, 469);
                player2Color = 6;
                break;

            }
            else if (mouseX >= 525 && mouseX <= 575 &&
                     mouseY >= 420 && mouseY <= 470)   /// LIGHTBLUE
            {

                setcolor(YELLOW);
                rectangle(525, 420, 575, 470);
                rectangle(526, 421, 574, 469);
                player2Color = 9;
                break;

            }
            else if (mouseX >= 225 && mouseX <= 275 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTGREEN
            {

                setcolor(YELLOW);
                rectangle(225, 520, 275, 570);
                rectangle(226, 521, 274, 569);
                player2Color = 10;
                break;

            }
            else if (mouseX >= 325 && mouseX <= 375 &&
                     mouseY >= 520 && mouseY <= 570)   /// GREEN
            {

                setcolor(YELLOW);
                rectangle(325, 520, 375, 570);
                rectangle(326, 521, 374, 569);
                player2Color = 2;
                break;

            }
            else if (mouseX >= 425 && mouseX <= 475 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTMAGENTA
            {

                setcolor(YELLOW);
                rectangle(425, 520, 475, 570);
                rectangle(426, 521, 474, 569);
                player2Color = 13;
                break;

            }
            else if (mouseX >= 525 && mouseX <= 575 &&
                     mouseY >= 520 && mouseY <= 570)   /// LIGHTCYAN
            {

                setcolor(YELLOW);
                rectangle(525, 520, 575, 570);
                rectangle(526, 521, 574, 569);
                player2Color = 11;
                break;

            }
        }
    }

// <- initialization ->

    settextstyle(4, 0, 3);
    setcolor(YELLOW);

// <- button ->

    int meniuX1 = 370, meniuY1 = 700, meniuX2, meniuY2;

    if(language == 0)
        button(meniuX1, meniuY1, meniuX2, meniuY2, 50, 20, "Meniu");
    else
        button(meniuX1, meniuY1, meniuX2, meniuY2, 50, 20, "Menu");

    setcolor(BLACK);
    rectangle(335, 695, 450, 725);

// <- wait click ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();
            if (mouseX >=335 && mouseX <= 450 &&
                    mouseY >= 695 && mouseY <= 725)
                menu();
        }
    }
}

void menu() /// Frontend Menu
{

    // <- loading if game is started for first time ->

    if(loadingVal == 0) loadingScreen();

    // <- initialization ->

    setbkcolor(3);
    cleardevice();
    setcolor(LIGHTGRAY);
    settextstyle(4, 0, 4);
    setcolor(WHITE);

    readimagefile("logoblue.jpg", 210, 90, 590, 185);

    // <- buttons ->

    int joacaX1 = 370, joacaY1 = 320, joacaX2, joacaY2;

    if(language == 0)
        button(joacaX1, joacaY1, joacaX2, joacaY2, 50, 20, "Joaca PvP");
    else
        button(joacaX1, joacaY1, joacaX2, joacaY2, 50, 20, "Play PvP");

    int joaca2X1 = 370, joaca2Y1 = 370, joaca2X2, joaca2Y2;

    if(language == 0)
        button(joaca2X1, joaca2Y1, joaca2X2, joaca2Y2, 50, 20, "Joaca PvC");
    else
        button(joaca2X1, joaca2Y1, joaca2X2, joaca2Y2, 50, 20, "Play PvC");

    int tutorialX1 = 370, tutorialY1 = 420, tutorialX2, tutorialY2;

    button(tutorialX1, tutorialY1, tutorialX2, tutorialY2, 50, 20, "Tutorial");

    int temeX1 = 370, temeY1 = 470, temeX2, temeY2;

    if(language == 0)
        button(temeX1, temeY1, temeX2, temeY2, 50, 20, "Tema");
    else
        button(temeX1, temeY1, temeX2, temeY2, 50, 20, "Theme");


    int limbaX1 = 370, limbaY1 = 520, limbaX2, limbaY2;

    if(language == 0)
        button(limbaX1, limbaY1, limbaX2, limbaY2, 50, 20, "Limba");
    else
        button(limbaX1, limbaY1, limbaX2, limbaY2, 50, 20, "Language");

    int iesireX1 = 370, iesireY1 = 570, iesireX2, iesireY2;

    if(language == 0)
        button(iesireX1, iesireY1, iesireX2, iesireY2, 50, 20, "Iesire");
    else
        button(iesireX1, iesireY1, iesireX2, iesireY2, 50, 20, "Quit");


    setcolor(BLACK);

    if(language == 0)
    {

        joacaX1 = 270;
        joacaX2 = 515;
        joacaY1 = 315;
        joacaY2 = 345;
        joaca2X1 = joacaX1;
        joaca2X2 = joacaX2;
        joaca2Y1 = 365;
        joaca2Y2 = 395;
        tutorialX1 = 300;
        tutorialX2 = 485;
        tutorialY1 = 415;
        tutorialY2 = 445;
        temeX1 = limbaX1 = iesireX1 = 315;
        temeX2 = limbaX2 = iesireX2 = 470;
        temeY1 = 465;
        temeY2 = 495;
        limbaY1 = 515;
        limbaY2 = 545;
        iesireY1 = 565;
        iesireY2 = 595;

    }

    else
    {

        joacaX1 = 275;
        joacaX2 = 510;
        joacaY1 = 315;
        joacaY2 = 345;
        joaca2X1 = joacaX1;
        joaca2X2 = joacaX2;
        joaca2Y1 = 365;
        joaca2Y2 = 395;
        tutorialX1 = 300;
        tutorialX2 = 485;
        tutorialY1 = 415;
        tutorialY2 = 445;
        temeX1 = 310;
        temeX2 = 475;
        limbaX1 = 285;
        limbaX2 = 505;
        iesireX1 = 335;
        iesireX2 = 450;
        temeY1 = 465;
        temeY2 = 495;
        limbaY1 = 515;
        limbaY2 = 545;
        iesireY1 = 565;
        iesireY2 = 595;

    }

    rectangle(joacaX1, joacaY1, joacaX2, joacaY2);
    rectangle(joaca2X1, joaca2Y1, joaca2X2, joaca2Y2);
    rectangle(tutorialX1, tutorialY1, tutorialX2, tutorialY2);
    rectangle(temeX1, temeY1, temeX2, temeY2);
    rectangle(limbaX1, limbaY1, limbaX2, limbaY2);
    rectangle(iesireX1, iesireY1, iesireX2, iesireY2);

    // <- wait click ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();
            if (mouseX >=joacaX1 && mouseX <= joacaX2 &&
                    mouseY >= joacaY1 && mouseY <= joacaY2)
            {
                comingFromPVC = false;
                selectlevel();
            }
            else if(mouseX >= joaca2X1 && mouseX <= joaca2X2 &&
                    mouseY >= joaca2Y1 && mouseY <= joaca2Y2)
            {
                comingFromPVC = true;
                selectlevel();
            }
            else if(mouseX >= tutorialX1 && mouseX <= tutorialX2 &&
                    mouseY >= tutorialY1 && mouseY <= tutorialY2)
                afistutorial();
            else if(mouseX >= temeX1 && mouseX <= temeX2 &&
                    mouseY >= temeY1 && mouseY <= temeY2)
                afisteme();
            else if(mouseX >= limbaX1 && mouseX <= limbaX2 &&
                    mouseY >= limbaY1 && mouseY <= limbaY2)
                afislimba();
            else if(mouseX >= iesireX1 && mouseX <= iesireX2 &&
                    mouseY >= iesireY1 && mouseY <= iesireY2)
                closegraph();
        }
    }

}

void sfarsit() /// Frontend end
{
    // <- initialization ->

    setbkcolor(3);
    cleardevice();
    readimagefile("logoblue.jpg", 210, 90, 590, 185);
    settextstyle(4, 0, 5);

    // <- display winner ->

    for(int i=1; i<=5; i++)
    {
        if(player == 0)
        {

            if(comingFromPVC == false)
            {
                setcolor(WHITE);

                if(language == 0)
                {
                    outtextxy(80, 350, "Jucatorul 1 a castigat!!");
                    delay(125);

                    setcolor(RED);
                    outtextxy(80, 350, "Jucatorul 1 a castigat!!");
                    delay(125);
                }
                else
                {
                    outtextxy(190, 350, "Player 1 won!!");
                    delay(125);

                    setcolor(RED);
                    outtextxy(190, 350, "Player 1 won!!");
                    delay(125);
                }
            }

            else
            {
                if(language == 0)
                {
                    setcolor(WHITE);
                    outtextxy(230, 350, "Ai pierdut!!");
                    delay(125);

                    setcolor(RED);
                    outtextxy(230, 350, "Ai pierdut!!");
                    delay(125);
                }
                else
                {
                    setcolor(WHITE);
                    outtextxy(270, 350, "You lost!!");
                    delay(125);

                    setcolor(RED);
                    outtextxy(270, 350, "You lost!!");
                    delay(125);

                }
            }

        }

        else
        {

            if(comingFromPVC == false)
            {
                if(language == 0)
                {
                    setcolor(WHITE);
                    outtextxy(80, 350, "Jucatorul 2 a castigat!!");
                    delay(125);

                    setcolor(RED);
                    outtextxy(80, 350, "Jucatorul 2 a castigat!!");
                    delay(125);
                }
                else
                {
                    setcolor(WHITE);
                    outtextxy(190, 350, "Player 2 won!!");
                    delay(125);

                    setcolor(RED);
                    outtextxy(190, 350, "Player 2 won!!");
                    delay(125);
                }
            }

            else
            {
                if(language == 0)
                {
                    setcolor(WHITE);
                    outtextxy(230, 350, "Ai castigat!!");
                    delay(125);

                    setcolor(RED);
                    outtextxy(230, 350, "Ai castigat!!");
                    delay(125);
                }
                else
                {
                    setcolor(WHITE);
                    outtextxy(250, 350, "You won!!");
                    delay(125);

                    setcolor(RED);
                    outtextxy(250, 350, "You won!!");
                    delay(125);
                }
            }

        }
    }

    // <- initialization ->

    delay(200);
    settextstyle(4, 0, 3);
    setcolor(YELLOW);

    // <- button ->

    int meniuX1 = 370, meniuY1 = 700, meniuX2, meniuY2;

    if(language == 0)
        button(meniuX1, meniuY1, meniuX2, meniuY2, 50, 20, "Meniu");
    else
        button(meniuX1, meniuY1, meniuX2, meniuY2, 50, 20, "Menu");

    setcolor(BLACK);
    rectangle(335, 695, 450, 725);

    // <- wait click ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();
            if (mouseX >=335 && mouseX <= 450 &&
                    mouseY >= 695 && mouseY <= 725)
                menu();
        }
    }

}

void selectlevel() /// Frontend level selection
{

    // <- initialization ->

    lives1=3, lives2=3;
    usedRandomMovePlayer1 = false;
    usedRandomMovePlayer2 = false;
    cleanup();
    player = 0;
    setbkcolor(3);
    cleardevice();
    settextstyle(4, 0, 2);
    readimagefile("logoblue.jpg", 210, 90, 590, 185);

    // <- buttons ->

    int random1X1 = 180, random1Y1 = 360, random1X2, random1Y2;
    int random2X1 = 180, random2Y1 = 380, random2X2, random2Y2;


    setcolor(WHITE);
    if(language == 0)
    {
        button(random1X1, random1Y1, random1X2, random1Y2, 50, 20, "Puncte generate");
        button(random2X1, random2Y1, random2X2, random2Y2, 50, 20, "de calculator");
    }
    else
    {
        button(random1X1, random1Y1, random1X2, random1Y2, 50, 20, "Dots automatically");
        button(random2X1, random2Y1, random2X2, random2Y2, 50, 20, "generated");
    }

    int introduse1X1 = 590, introduse1Y1 = 360, introduse1X2, introduse1Y2;
    int introduse2X1 = 590, introduse2Y1 = 380, introduse2X2, introduse2Y2;
    if(language == 0)
    {
        button(introduse1X1, introduse1Y1, introduse1X2, introduse1Y2, 50, 20, "Puncte introduse");
        button(introduse2X1, introduse2Y1, introduse2X2, introduse2Y2, 50, 20, "de jucatori");
    }
    else
    {
        button(introduse1X1, introduse1Y1, introduse1X2, introduse1Y2, 50, 20, "Dots entered");
        button(introduse2X1, introduse2Y1, introduse2X2, introduse2Y2, 50, 20, "by players");
    }

    setcolor(BLACK);
    rectangle(80, 360, 330, 400);
    rectangle(490, 360, 740, 400);

    // <- initialization ->

    settextstyle(4, 0, 3);
    setcolor(YELLOW);

    // <- button ->

    int meniuX1 = 370, meniuY1 = 700, meniuX2, meniuY2;

    if(language == 0)
        button(meniuX1, meniuY1, meniuX2, meniuY2, 50, 20, "Meniu");
    else
        button(meniuX1, meniuY1, meniuX2, meniuY2, 50, 20, "Menu");

    setcolor(BLACK);
    rectangle(335, 695, 450, 725);

    // <- wait click ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();
            if (mouseX >=335 && mouseX <= 450 &&
                    mouseY >= 695 && mouseY <= 725)
                menu();
            else if(mouseX >= 80 && mouseX <= 330 &&
                    mouseY >= 360 && mouseY <= 400)
                randomDots();
            else if(mouseX >= 490 && mouseX <= 740 &&
                    mouseY >= 360 && mouseY <= 400)
                getDots();
        }
    }

}


void randomDots() /// First Game Mode
{
    // <- initialization ->

    setbkcolor(backgroundColor);
    cleardevice();
    draw20RandomDots();
    blankError();
    setcolor(BLACK);

    // <- box ->

    rectangle(25, 90, 775, 750);
    rectangle(27, 92, 773, 748);

    // <- circles ->

    for(int i=250; i<=600; i=i+50)
    {
        circle(i,810, 27);
        circle(i,810, 25);
        circle(i,810, 23);
    }

    for(int i=250; i<=600; i=i+50)
    {
        circle(-10, i, 27);
        circle(810, i, 27);

        circle(-10, i, 25);
        circle(810, i, 25);

        circle(-10, i, 23);
        circle(810, i, 23);
    }

    // <- next step ->
    if(comingFromPVC == true)
        jocPVC();
    else joc();

}

void getDots() /// Second Game Mode
{
    // <- initialization ->

    setbkcolor(backgroundColor);
    cleardevice();
    blankError();
    if(comingFromPVC == false)
        showLives();
    else
        showLivesPVC();
    setcolor(BLACK);

    // <- box ->

    rectangle(25, 90, 775, 750);
    rectangle(27, 92, 773, 748);

    // <- circles ->

    for(int i=250; i<=600; i=i+50)
    {
        circle(i,810, 27);
        circle(i,810, 25);
        circle(i,810, 23);
    }

    for(int i=250; i<=600; i=i+50)
    {
        circle(-10, i, 27);
        circle(810, i, 27);

        circle(-10, i, 25);
        circle(810, i, 25);

        circle(-10, i, 23);
        circle(810, i, 23);
    }

    // <- info ->

    outtextxy(5,60,"                                                                                                                        ");
    if(comingFromPVC == true)
        if(language == 0)
            outtextxy(70,60,"Pentru a incepe te rog selecteaza 20 de puncte in chenar.");
        else
            outtextxy(140,60,"To start please select 20 points in the box.");
    else if(language == 0)
        outtextxy(75,60,"Pentru a incepe va rog selectati 20 de puncte in chenar.");
    else
        outtextxy(140,60,"To start please select 20 points in the box.");

    // <- initialization ->

    const int maxX = 700;
    const int maxY = 700;
    const int minX = 50;
    const int minY = 100;
    int remainingDots = 20;

    // <- get dots loop ->

    while(remainingDots != 0)
    {
        int xpunct, ypunct;
        while(!kbhit())
        {
            if(ismouseclick(WM_LBUTTONDOWN)==1)
            {
                clearmouseclick(WM_LBUTTONDOWN);
                xpunct = mousex();
                ypunct = mousey();
                break;
            }
        }

        bool validPoint = true;

        const int maxX = 773;
        const int maxY = 748;
        const int minX = 27;
        const int minY = 92;

        if(xpunct>=maxX or xpunct<=minX or ypunct>=maxY or ypunct<=minY)
        {
            outtextxy(5,60,"                                                                                                                                    ");

            if(language == 0)
                outtextxy(125,60,"Punctul dorit nu este in interiorul chenarului.");
            else
                outtextxy(145,60,"The desired point is not inside the box.");

            delay(2000);
            outtextxy(5,60,"                                                                                                                                     ");
            delay(200);
            validPoint = false;
        }

        if(validPoint == true)
            do
            {
                validPoint = true;  // Assume the point is valid until proven otherwise

                // Check the distance between the new point and existing points
                for (int j = 1; j < 20-remainingDots+1; j++)
                {
                    int dx = xpunct - listaNoduri[j].x;
                    int dy = ypunct - listaNoduri[j].y;
                    double distance = sqrt(dx * dx + dy * dy);

                    if (distance < 75)
                    {
                        validPoint = false;
                        break;  // Exit the loop early if the point is too close
                    }
                }
                if(validPoint == false)
                {
                    outtextxy(5,60,"                                                                                                                        ");

                    if(language == 0)
                        outtextxy(130,60,"Punctul dorit este prea aproape de alt punct.");
                    else
                        outtextxy(120,60,"The desired point is too close to another point.");


                    delay(2000);
                    outtextxy(5,60,"                                                                                         ");
                    delay(200);
                    break;
                }

            }
            while (!validPoint);

        if(validPoint == true)
        {
            graph[xpunct][ypunct] = 1;
            listaNoduri[20-remainingDots+1].x = xpunct;
            listaNoduri[20-remainingDots+1].y = ypunct;
            deseneazaNod(xpunct, ypunct, 20-remainingDots+1);
            listaNoduri[20-remainingDots+1].numar = 20-remainingDots+1;
            remainingDots --;
        }

        if(remainingDots == 20 )
        {
            outtextxy(5,60,"                                                                                                                        ");
            if(comingFromPVC == true)
                if(language == 0)
                    outtextxy(70,60,"Pentru a incepe te rog selecteaza 20 de puncte in chenar.");
                else
                    outtextxy(140,60,"To start please select 20 points in the box.");
            else if(language == 0)
                outtextxy(75,60,"Pentru a incepe va rog selectati 20 de puncte in chenar.");
            else
                outtextxy(140,60,"To start please select 20 points in the box.");
        }
        else
        {
            outtextxy(5,60,"                                                                                                                        ");

            if(language == 0)
                outtextxy(175,60,"Au mai ramas ");
            else
                outtextxy(195,60,"There are");

            char s[10];
            sprintf(s, "%d", remainingDots);
            outtextxy(340,60, s);

            if(language == 0)
                outtextxy(400,60, "puncte de selectat.");
            else
                outtextxy(390,60, "dots left to select.");

        }
    }

    // <- next step ->

    if(comingFromPVC == true)
        jocPVC();
    else joc();

}

void afislimba()
{
    setbkcolor(3);
    cleardevice();
    readimagefile("logoblue.jpg", 210, 90, 590, 185);
    settextstyle(4, 0, 2);

    if(language == 0)
    {
        outtextxy(250, 230, "Selecteaza limba dorita");
        outtextxy(190, 250, "daca doresti sa modifici alegerea");
        outtextxy(160, 270, "finalizeaza selectia si revino in meniu.");
    }
    else
    {
        outtextxy(230, 230, "Select the desired language");
        outtextxy(190, 250, "if you want to modify the selection");
        outtextxy(125, 270, "complete the selection and return to the menu.");
    }


    setfillstyle(SOLID_FILL, WHITE);
    bar(270, 395, 335, 425);
    bar(470, 395, 535, 425);

    setcolor(BLACK);
    setbkcolor(WHITE);
    outtextxy(280, 400, "EN");
    outtextxy(480, 400, "RO");


    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();

            if (mouseX >= 270 && mouseX <= 335 &&
                    mouseY >= 395 && mouseY <= 425)   /// EN
            {

                setcolor(BLACK);
                rectangle(270, 395, 335, 425);
                rectangle(271, 396, 334, 424);
                language = 1;
                break;

            }
            else if (mouseX >= 470 && mouseX <= 535 &&
                     mouseY >= 395 && mouseY <= 425)   /// RO
            {

                setcolor(BLACK);
                rectangle(470, 395, 535, 425);
                rectangle(471, 396, 534, 424);
                language = 0;
                break;

            }
        }
    }

    // <- initialization ->

    settextstyle(4, 0, 3);
    setcolor(YELLOW);
    setbkcolor(3);

    // <- button ->

    int meniuX1 = 370, meniuY1 = 700, meniuX2, meniuY2;

    if(language == 0)
        button(meniuX1, meniuY1, meniuX2, meniuY2, 50, 20, "Meniu");
    else
        button(meniuX1, meniuY1, meniuX2, meniuY2, 50, 20, "Menu");

    setcolor(BLACK);
    rectangle(335, 695, 450, 725);

    // <- wait click ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();
            if (mouseX >=335 && mouseX <= 450 &&
                    mouseY >= 695 && mouseY <= 725)
                menu();
        }
    }


}

void afistutorial() /// Frontend Tutorial
{

    // <- initialization ->

    setbkcolor(3);
    cleardevice();
    readimagefile("logoblue.jpg", 210, 90, 590, 185);
    settextstyle(4, 0, 4);

    // <- info ->

    if(language == 0)
    {
        outtextxy(250, 250, "Cum se joaca?");

        settextstyle(4, 0, 1);
        outtextxy(120, 350, "Sunt doua moduri de joc: punctele pot fi");
        outtextxy(120, 370, "generate automat sau introduse manual de");
        outtextxy(120, 390, "catre jucatori.");

        outtextxy(120, 430, "La inceput fiecare jucator are 3 vieti.");
        outtextxy(120, 450, "Jucatorii traseaza pe rand cate un");
        outtextxy(120, 470, "segment intre doua puncte de pe ecran.");
        outtextxy(120, 490, "Daca segmentul pe care doresc sa il");
        outtextxy(120, 510, "traseze intersecteaza alt segment, sau");
        outtextxy(120, 530, "alt punct de pe ecran, acesta pierde o");
        outtextxy(120, 550, "viata. Cand unul dintre jucatori ramane");
        outtextxy(120, 570, "fara vieti, acesta pierde. Succes!");
    }

    else
    {
        outtextxy(260, 250, "How to play?");

        settextstyle(4, 0, 1);
        outtextxy(110, 350, "There are two ways of playing: the points can be");
        outtextxy(110, 370, "automatically generated or manually entered by");
        outtextxy(110, 390, "the players.");

        outtextxy(110, 430, "At the beginning each player has 3 lives.");
        outtextxy(110, 450, "players take turns drawing one");
        outtextxy(110, 470, "segment between two points on the screen.");
        outtextxy(110, 490, "If the segment they want to");
        outtextxy(110, 510, "draw intersect another segment, or");
        outtextxy(110, 530, "another point on the screen, he loses one");
        outtextxy(110, 550, "life. When one of the players remains");
        outtextxy(110, 570, "without lives, he loses. Good luck!");
    }


    // <- initialization ->

    settextstyle(4, 0, 3);
    setcolor(YELLOW);

    // <- button ->

    int meniuX1 = 370, meniuY1 = 700, meniuX2, meniuY2;

    if(language == 0)
        button(meniuX1, meniuY1, meniuX2, meniuY2, 50, 20, "Meniu");
    else
        button(meniuX1, meniuY1, meniuX2, meniuY2, 50, 20, "Menu");

    setcolor(BLACK);
    rectangle(335, 695, 450, 725);

    // <- wait click ->

    while(!kbhit())
    {
        if(ismouseclick(WM_LBUTTONDOWN)==1)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int mouseX = mousex();
            int mouseY = mousey();
            if (mouseX >=335 && mouseX <= 450 &&
                    mouseY >= 695 && mouseY <= 725)
                menu();
        }
    }

}


void button(int &x1, int &y1, int &x2, int &y2, int width, int height, char *text) /// Draw button
{

    // <- info ->

    outtextxy(x1+width/2-textwidth(text)/2, y1+height/2-textheight(text)/2, text);

    // <- initialization ->

    x1 = x1+width/2-textwidth(text)/2;
    x2 = x1+width/2+textwidth(text)/2;
    y1 = y1+height/2-textheight(text)/2;
    y2 = y1+height/2+textheight(text)/2;

}

void blankError() /// Delete info
{

    outtextxy(20,20,"                                                                                                                         ");

}

void showLives() /// Lives display
{

    // <- design ->

    setcolor(WHITE);
    line(400, 15, 400, 50);

    // <- info ->

    setcolor(player1Color);
    if(player==0) setcolor(YELLOW);
    settextstyle(EUROPEAN_FONT,HORIZ_DIR,2);

    if(language == 0)
        outtextxy(60,20,"Jucatorul 1:");
    else
        outtextxy(60,20,"Player 1:");

    char s[10];
    sprintf(s, "%d", lives1);
    setcolor(RED);
    outtextxy(230,20,s);

    if(language == 0)
        outtextxy(250,20,"vieti");
    else
        outtextxy(250,20,"lives");

    setcolor(player2Color);
    if(player==1) setcolor(YELLOW);

    if(language == 0)
        outtextxy(480,20,"Jucatorul 2:");
    else
        outtextxy(480,20,"Player 2:");

    sprintf(s, "%d", lives2);
    setcolor(RED);
    outtextxy(650,20,s);

    if(language == 0)
        outtextxy(670,20,"vieti");
    else
        outtextxy(670,20,"lives");

    // <- design ->

    setcolor(WHITE);
    line(20, 50, 780, 50);

}

void showLivesPVC() /// Lives display
{

    // <- info ->

    setcolor(player1Color);
    settextstyle(EUROPEAN_FONT,HORIZ_DIR,2);

    if(language == 0)
        outtextxy(330,20,"Mai ai:");
    else
        outtextxy(290,20,"You have:");

    char s[10];
    sprintf(s, "%d", lives1);
    setcolor(RED);
    outtextxy(420,20,s);

    if(language == 0)
        outtextxy(440,20,"vieti");
    else
        outtextxy(440,20,"lives left");

    // <- design ->

    setcolor(WHITE);
    line(20, 50, 780, 50);

}

void deseneazaNod(int x, int y,int i) /// Draw point
{

    setfillstyle(SOLID_FILL, dotsColor);
    bar(x-5, y-5, x+5, y+5);

}

void draw20RandomDots() /// Random Points
{

    srand(static_cast<unsigned int>(time(nullptr)));

    const int maxX = 700;
    const int maxY = 700;
    const int minX = 50;
    const int minY = 100;

    for (int i = 1; i <= 20; i++)
    {
        int xpunct, ypunct;
        bool validPoint;

        do
        {
            validPoint = true;  // Assume the point is valid until proven otherwise

            xpunct = rand() % (maxX - minX) + minX;
            ypunct = rand() % (maxY - minY) + minY;

            // Check the distance between the new point and existing points
            for (int j = 1; j < i; j++)
            {
                int dx = xpunct - listaNoduri[j].x;
                int dy = ypunct - listaNoduri[j].y;
                double distance = sqrt(dx * dx + dy * dy);

                if (distance < 100)
                {
                    validPoint = false;
                    break;  // Exit the loop early if the point is too close
                }
            }
        }
        while (!validPoint);

        graph[xpunct][ypunct] = 1;
        listaNoduri[i].x = xpunct;
        listaNoduri[i].y = ypunct;
        deseneazaNod(xpunct, ypunct, i);
        listaNoduri[i].numar = i;
    }

}

void randomMove() /// Random Move
{
    int mutare1, mutare2;
    noPossibleMove = false;

    for(int numarare=1; (numarare<=20 && amGasit == false); numarare++)
    {
        for(int numarare0=1; (numarare0<=20 && numarare!=numarare0 && amGasit == false); numarare0++)
        {
            SePoateRandom(numarare,numarare0);
        }
    }
    if(amGasit == false)
    {
        noPossibleMove = true;
    }
}

void cleanup() /// Structure Cleanup
{

    for(int i=0; i<=20; i++)
    {
        listaNoduri[i].x=0;
        listaNoduri[i].y=0;
        listaNoduri[i].numar=0;
        v[i].nod1.x=0;
        v[i].nod1.y=0;
        v[i].nod2.x=0;
        v[i].nod2.y=0;
        v[i].nod1.numar=0;
        v[i].nod2.numar=0;
    }

}

void trasareSegment(int  coord1,int coord2) /// Draw line - add data
{
    if(player==0) setcolor(player1Color);
    else setcolor(player2Color);
    line(listaNoduri[coord1].x,listaNoduri[coord1].y,listaNoduri[coord2].x,listaNoduri[coord2].y);
    v[MENTOSAN].nod1.x=listaNoduri[coord1].x;
    v[MENTOSAN].nod1.y=listaNoduri[coord1].y;
    v[MENTOSAN].nod2.x=listaNoduri[coord2].x;
    v[MENTOSAN].nod2.y=listaNoduri[coord2].y;
    MENTOSAN++;
}

// -- Calculus section -----------------------------------------------------------------------------------------------------------------------------------------------------------------

int det(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{

    return (X2 - X1) * (Y3 - Y1) - (X3 - X1) * (Y2 - Y1);

}

int pctsegm(int x1, int y1, int x2, int y2, int x3, int y3)
{

    int d = det(x1, y1, x2, y2, x3, y3);

    if (d != 0)
        return 0;
    if (x1 == x3 && y1 == y3)
        return 1;
    if (x2 == x3 && y2 == y3)
        return 1;
    if ((x3 - x1) * (x3 - x2) < 0 || (y3 - y1) * (y3 - y2) < 0)
        return 1;
    else
        return 0;

}

bool SePoate(int nod1, int nod2)
{

    float d1, d2, d3, d4;
    BOMBOCLAT=0;
    nod a = listaNoduri[nod1];
    nod b = listaNoduri[nod2];

    for(int i=0; i<MENTOSAN; i++)
    {
        nod c = v[i].nod1;
        nod d = v[i].nod2;

        if (pctsegm(a.x, a.y, b.x, b.y, d.x, d.y) || pctsegm(c.x, c.y, d.x, d.y, a.x, a.y) || pctsegm(c.x, c.y, d.x, d.y, b.x, b.y) || pctsegm(a.x, a.y, b.x, b.y, c.x, c.y) )
        {
            if(player == 0)
            {
                lives1--;
            }
            else
            {
                lives2--;
            }
            outtextxy(5,60,"                                                                                               ");
            outtextxy(5,60,"Nu poti trasa un segment aici. Incearca din nou in cateva momente!");
            delay(2000);
            outtextxy(5,60,"                                                                                         ");
            delay(200);


            if(comingFromPVC == false)
                joc();
            else
                jocPVC();

            return 0;

        }
        else
        {
            d1 = det(c.x, c.y, d.x, d.y, a.x, a.y);
            d2 = det(c.x, c.y, d.x, d.y, b.x, b.y);
            d3 = det(a.x, a.y, b.x, b.y, c.x, c.y);
            d4 = det(a.x, a.y, b.x, b.y, d.x, d.y);

            if (d1 * d2 < 0 && d3 * d4 < 0)
            {
                if(player == 0)
                {
                    lives1--;
                }
                else
                {
                    lives2--;
                }
                outtextxy(5,60,"                                                                                               ");
                outtextxy(5,60,"Nu poti trasa un segment aici. Incearca din nou in cateva momente!");
                delay(2000);
                outtextxy(5,60,"                                                                                         ");
                delay(200);

                if(comingFromPVC == false)
                    joc();
                else
                    jocPVC();

                return 0;

            }
            else
            {
                BOMBOCLAT++;
            }
        }
    }
    if(BOMBOCLAT==MENTOSAN)
    {
        amGasit = true;
        trasareSegment(nod1, nod2);
    }
    return 0;

}

bool SePoateRandom(int nod1, int nod2)
{

    float d1, d2, d3, d4;
    BOMBOCLAT=0;
    nod a = listaNoduri[nod1];
    nod b = listaNoduri[nod2];

    for(int i=0; i<MENTOSAN; i++)
    {
        nod c = v[i].nod1;
        nod d = v[i].nod2;

        if (pctsegm(a.x, a.y, b.x, b.y, d.x, d.y) || pctsegm(c.x, c.y, d.x, d.y, a.x, a.y) || pctsegm(c.x, c.y, d.x, d.y, b.x, b.y) || pctsegm(a.x, a.y, b.x, b.y, c.x, c.y) )
            return 0;
        else
        {
            d1 = det(c.x, c.y, d.x, d.y, a.x, a.y);
            d2 = det(c.x, c.y, d.x, d.y, b.x, b.y);
            d3 = det(a.x, a.y, b.x, b.y, c.x, c.y);
            d4 = det(a.x, a.y, b.x, b.y, d.x, d.y);

            if (d1 * d2 < 0 && d3 * d4 < 0)
                return 0;
            else
                BOMBOCLAT++;
        }
    }
    if(BOMBOCLAT==MENTOSAN)
    {
        amGasit = true;
        trasareSegment(nod1, nod2);
    }
    return 0;

}

/// SEGMENTE - TUDOR ALEXANDRU, URSACHE STEFAN
