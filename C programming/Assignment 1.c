#include <stdio.h>
#include <string.h>

int main()
{
    printf("\t \t \t \t \t \t Timetable-Monsoon 2018-V1.4(Aug-Dec 2018) (made in IIIT SRICITY) \n");
    prints1();
    printf("\t \t \t \t \t \t \t \t \t    UG1 \n");
    prints1();
    printf("S NO.|"); printf("Timings           |"); printf("        Monday         |");printf("       Tuesday         |");printf("      Wednesday        |");printf("       Thursday        |");printf("        Friday         |");printf("      Saturday      |\n");
    prints1();
    char sn1[13][50]={"1","2","3","4","5","6","7","8","9","10","11","12","13"};
   char time1[13][50]=  {"09:00-10:00","           ","10:00-11:00","           ","11:00-11:15","11:15-12:15","           ","12:15-13:30","13:30-14:30","           ","14:30-15:30","15:30-16:30","16:30-17:30"};
   char monday1[13][50]={"PC Sec A-113        ","Comm Skills-1 Sec B ","Comm Skills-1 Sec A ","PC Sec B-115        ","Short Break         ","DEC Sec A-113       ","OC Sec B-115        ","Lunch Break         ","OC Sec A-113        ","DEC Sec B-115       ","PC Lab A            ","                    ","                    "};
  char tuesday1[13][50]={"DMPT-Tut Sec A-113  ","                    ","DMPT Sec A-113      ","                    ","Short Break         ","DMPT Sec B-115      ","                    ","Lunch Break         ","                    ","                    ","OC Lab A            ","DEC Lab B           ","                    "};
char wednesday1[13][50]={"DEC Sec A-113       ","OC Sec B-115        ","OC Sec A-113        ","DEC Sec B-115       ","Short Break         ","PC Sec A-113        ","Comm Skills-1 Sec B ","Lunch Break         ","Comm Skills-1 Sec A ","PC Sec B-115        ","PC Lab B            ","                    ","                    "};
 char thursday1[13][50]={"DMPT-Tut Sec B-115  ","                    ","DMPT Sec A-113      ","                    ","Short Break         ","DMPT Sec B-115      ","                    ","Lunch Break         ","DMPT-Tut Sec A-113  ","                    ","OC Lab B            ","DEC Lab C           ","                    "};
   char friday1[13][50]={"DMPT Sec A-113      ","Comm Skills-1 Sec B ","Comm Skills-1 Sec A ","DMPT Sec B-115      ","Short Break         ","DEC Sec A-113       ","OC Sec B-115        ","Lunch Break         ","OC Sec A-113        ","DEC Sec B-115       ","PC Lab A            ","                    ","                    "};
 char saturday1[13][50]={"PC Lab B            ","                    ","PC Lab B            ","                    ","Short Break         ","PC Lab B            ","                    ","Lunch Break         ","DMPT-Tut Sec B 115  ","                    ","OC Lab C            ","DEC Lab A           ","                    "};
   //printing lines ug1
   for(int i=0;i<13;i++)
   {    printf("%-5s|",sn1[i]);
        printf("%s       |",time1[i]);
        printf("%s   |",monday1[i]);
        printf("%s   |",tuesday1[i]);
        printf("%s   |",wednesday1[i]);
        printf("%s   |",thursday1[i]);
        printf("%s   |",friday1[i]);
        printf("%s|",saturday1[i]);
        printf("\n");
        if((i==1)||(i==3)||(i==4)||(i==7)||(i==9)||(i==12) || (i==6))
            {  prints1(); }
   }
    prints1();

    /**ug1 over -----------------------------------------------------------------------------------------------------------------------------------------*/

    printf("\n\n\n");
    prints1();
    printf("\t \t \t \t \t \t \t \t \t   UG2 \n");
    prints1();
    printf("S NO.|"); printf("Timings           |"); printf("         Monday        |");printf("        Tuesday        |");printf("       Wednesday       |");printf("        Thursday       |");printf("         Friday        |");printf("      Saturday      |\n");
    prints1();
    char sn2[15][50]={"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15"};
   char time2[15][50]={"09:00-10:00","           ","10:00-11:00","           ","11:00-11:15","11:15-12:15","           ","12:15-13:15","           ","13:15-14:30","14:30-15:30","           ","15:30-16:30","           ","16:30-17:30"};
   char monday2[15][50]={"Algo-Sec-A          ","                    ","DSAA Sec-A          ","Algo-Sec-B          ","Short Break         ","OS Sec-A            ","ASE-1 Sec-B         ","ASE-1 Sec-A         ","OS Sec-B            ","Lunch Break         ","                    ","DSAA Lab Sec-A      ","ASE-1 Lab Sec-B     ","                    ","                    "};
  char tuesday2[15][50]={"Maths-3 Sec-B       ","Comm Skills-3 Sec-A ","Comm Skills-3 Sec-B "," Maths-3 Sec-A      ","Short Break         ","DSAA Sec-B          ","                    ","                    ","                    ","Lunch Break         ","                    ","                    ","                    ","                    ","                    "};
char wednesday2[15][50]={"Comm Skills-3 Sec-A ","OS Sec-B            ","OS Sec-A            ","Comm Skills-3 Sec-B ","Short Break         ","Algo Sec-A          ","DSAA Sec-B          ","DSAA Sec-A          ","Algo Sec-B          ","Lunch Break         ","                    ","DSAA Lab Sec-B      ","ASE-1 Lab Sec-A     ","                    ","                    "};
 char thursday2[15][50]={"Maths-3 Sec-B       ","Comm Skills-3 Sec-A ","Comm Skills-3 Sec-B ","Maths-3 Sec-A       ","Short Break         ","Algo Sec-A          ","DSAA Tut Sec-B      ","DSAA Tut Sec-A      ","Algo Sec-B          ","Lunch Break         ","                    ","                    ","                    ","                    ","                    "};
   char friday2[15][50]={"Maths-3 Sec-B       ","DSAA Tut Sec-A      ","DSAA Tut Sec-B      ","Maths-3 Sec-A       ","Short Break         ","OS Sec-A            ","ASE-1 Sec-B         ","ASE-1 Sec-A         ","OS Sec-B            ","Lunch Break         ","OS Sec-A            ","Algo Sec-B          ","Algo Sec-A          ","OS Sec-B            ","                    "};
 char saturday2[15][50]={"Maths-3 Sec-B       ","                    ","Maths-3 Sec-A       ","                    ","Short Break         ","ASE-1 Sec-A         ","                    ","ASE-1 Sec-B         ","                    ","Lunch Break         ","                    ","                    ","                    ","                    ","                    "};
   //printing lines ug2
   for(int i=0;i<15;i++)
   {    printf("%-5s|",sn2[i]);
        printf("%s       |",time2[i]);
        printf("%s   |",monday2[i]);
        printf("%s   |",tuesday2[i]);
        printf("%s   |",wednesday2[i]);
        printf("%s   |",thursday2[i]);
        printf("%s   |",friday2[i]);
        printf("%s|",saturday2[i]);
        printf("\n");
       if((i==6)||(i==1)||(i==3)||(i==4)||(i==8)||(i==9)||(i==14))
       {  prints1(); }
    }
    prints1();
    /**ug2 over -----------------------------------------------------------------------------------------------------------------------------------------*/

    printf("\n\n\t \t \t \t \t \t \t \t \tUG-3 / UG-4 / MS / PhD \n");
    prints1();

    printf("S NO. |"); printf("Timings          |"); printf("        Monday        |");printf("       Tuesday         |");printf("      Wednesday         |");printf("       Thursday        |");printf("        Friday         |");printf("      Saturday      |\n");
    prints1();
    char sn3[18][50]={"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18"};
   char time3[18][50]={"09:00-10:00","           ","10:00-11:00","           ","11:00-11:15","11:15-12:15","           ","12:15-13:15","           ","13:15-14:30","14:30-15:30","           ","15:30-16:30","           ","16:30-17:30","           ","17:30-18:30","           "};
   char monday3[18][50]={"DM-201            ","                  ","IOT-203           ","                  ","Short Break       ","Digicomm-201      ","                  ","IR-113            ","OS Sec-B          ","Lunch Break       ","CSD-115           ","                  ","DIP-115           ","                  ","EMT-203           ","                  ","DIP-115           ","                  "};
  char tuesday3[18][50]={"CSD-115           ","                  ","MERS-201          ","                  ","Short Break       ","SDS-101           ","DE-113            ","                  ","                  ","Lunch Break       ","SDS-101           ","DE-115            ","TOC-101           ","Digicomm-201      ","GE /LS /APS       ","203/101/201       ","IR-101            ","VLSI-203          "};
char wednesday3[18][50]={"IOT-203           ","                  ","DM-201            ","VLSI-203          ","Short Break       ","EMT-203           ","                  ","IOT-203           ","Algo Sec-B        ","Lunch Break       ","SDS-101           ","DE-115            ","TOC-101           ","Digicomm-201      ","DIP-115           ","                  ","YW-201            ","                  "};
 char thursday3[18][50]={"TOC-113           ","Digicomm-201      ","MERS-203          ","                  ","Short Break       ","EMT-203           ","                  ","SDS-113           ","Algo Sec-B        ","Lunch Break       ","IR-101            ","VLSI-203          ","IR-101            ","                  ","GE/LS             ","203/101           ","CSD-115           ","                  "};
   char friday3[18][50]={"DM-201            ","VLSI-2013         ","MERS-203          ","                  ","Short Break       ","ASE2 Sec A-113    ","                  ","ASE2 Sec B-113    ","OS Sec-B          ","Lunch Break       ","DIP-115           ","                  ","CSD-115           ","                  ","IOT-203           ","                  ","YW-201            ","                  "};
 char saturday3[18][50]={"ASE2 ProjectHr-115","                  ","ASE2 ProjectHr-115","MERS-203 / DM-201 ","Short Break       ","FA-103            ","ASE2 ProjectHr-115","FA-103            ","ASE2 ProjectHr-115","Lunch Break       ","ASE2 ProjectHr-115","APT-103           ","ASE2 ProjectHr-115","APT-103           ","TOC-101           ","EMT-203           ","                  ","                  "};
   //printing lines ug3
   for(int i=0;i<18;i++)
   {    printf("%-4s  |",sn3[i]);
        printf("%s      |",time3[i]);
        printf("%s    |",monday3[i]);
        printf("%s     |",tuesday3[i]);
        printf("%s      |",wednesday3[i]);
        printf("%s     |",thursday3[i]);
        printf("%s     |",friday3[i]);
        printf("%s  |",saturday3[i]);
        printf("\n");
       if((i==1)||(i==3)||(i==4)||(i==6)||(i==8)||(i==9)||(i==11)||(i==13)||(i==15))
        { prints1(); }

   }//for loop closed
   // ug3 over
    prints1();
    printf("\n");
    line();
    char a[30][20]={"Abbreviation","DMPT","PC","DEC","OC","Comm Skills-1","Maths-3","Algo","Comm Skills-3","DSAA","OS","ITWS-3","IOT","DIP","EMT","CSD","TOC","Digicomm","ITS","MERS","SDS","DE","GE","BIO","TW","VLSI","APS","IR","YW","APT"};
    char b[30][70] ={"Course Name","Discrete Maths and Probability Theory","Programming in C","Digital Design and Electronic Circuits","Overview of Computers","Communication Skills-1","Mathematics-3","Algorithms","Communication Skills-3","Digital Signal Analysis and Applicaitons","Operating Systems","IT Workshop-3","Internet of Things","Digital Image Processing","Electromagnetic Theory","Communication Systems Design","Theory of Computation","Digital Communication Systems","IT Systems","Microwave Engineering & Radar Systems","Statics for Data Science","Differential Equations","Good Earth","Bioscience","Technical Writing","Introduction to VLSI","Advanced Problem Solving","Information Retrieval","You and the World","Aptitude"};
    for (int i=0;i<29;i++)
    {
        printf("|  %-16s|",a[i]);
        printf("%-45s|",b[i]);
        printf("\n");
        if (i==0)
        line();
    }
    line();
    /*abbreviation over-----------------------------------------------------------------------------------------------------------------------------------------*/

  // formatting time table starts

    int chk=1;
    while (chk==1)
  {printf("Do You Want to Make any changes to the Time-Table? \n");
    printf("Enter 0 for no and 1 for yes!!\t");
    scanf("%d",&chk);
       // INPUT CONTROL
       while((chk!=1)&&(chk!=0))
       {printf("Please reenter!!  ");
        scanf("%d",&chk);
       }
    if(chk==1)
    {printf("For which time table do you want to make the changes?\nEnter \n1 for UG-1 \n2 for UG-2 \n3 for UG-3/UG-4/MS/PHD\n");
    int b;
    scanf("%d",&b);
    while ((b!=1) && (b!=2)&&(b!=3))
    {
        printf("Please re-enter:  ");
        scanf("%d",&b);
    }
    switch (b)
    { case 1:
        /**UG-1 case starting --------------------------------------------------------------------------------------------------------------------------------*/


            {printf("Enter\n");
            printf("1 for Monday \n2 for Tuesday \n3 for Wednesday \n4 for Thursday \n5 for Friday \n6 for Saturday \n");
            printf("Please Enter Your Choice   ");
            int a,mon;
            scanf("%d",&a);

        while((a<1)||(a>6))
        {    printf("Please reenter your choice!!  ");
             scanf("%d",&a);
        }

        switch(a)
        {
            case 1:mon=time_cell1();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",monday1[mon]);
                   break;

            case 2:mon=time_cell1();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",tuesday1[mon]);
                   break;

            case 3:mon=time_cell1();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",wednesday1[mon]);
                   break;

            case 4:mon=time_cell1();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",thursday1[mon]);
                   break;

            case 5:mon=time_cell1();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",friday1[mon]);
                   break;
            case 6:mon=time_cell1();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",saturday1[mon]);
                   break;

        }
        //printing starts
    prints1();
    printf("\t \t \t \t \t \t \t \t \t    UG1 \n");
    prints1();
    printf("S NO.|"); printf("Timings           |"); printf("        Monday         |");printf("       Tuesday         |");printf("      Wednesday        |");printf("       Thursday        |");printf("        Friday         |");printf("      Saturday      |\n");
    prints1();

    for(int i=0;i<13;i++)
   {    printf("%-5s|",sn1[i]);
        printf("%s       |",time1[i]);
        printf("%-20s   |",monday1[i]);
        printf("%-20s   |",tuesday1[i]);
        printf("%-20s   |",wednesday1[i]);
        printf("%-20s   |",thursday1[i]);
        printf("%-20s   |",friday1[i]);
        printf("%-20s|",saturday1[i]);
        printf("\n");
        if((i==1)||(i==3)||(i==4)||(i==7)||(i==9)||(i==12) || (i==6))
            {  prints1(); }
    }
break; // ug1 reprinting over
}//case closed

/**UG-2 case starting --------------------------------------------------------------------------------------------------------------------------------*/
     case 2:
         {printf("Enter\n");
            printf("1 for Monday \n2 for Tuesday \n3 for Wednesday \n4 for Thursday \n5 for Friday \n6 for Saturday \n");
            printf("Please Enter Your Choice   ");
            int z=0,mon;
            scanf("%d",&z);

        while((z<1)||(z>6))
        {    printf("Please reenter your choice!!  ");
             scanf("%d",&z);
        }

        switch(z)
        {
            case 1:mon=time_cell2();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",monday2[mon]);
                   break;

            case 2:mon=time_cell2();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",tuesday2[mon]);
                   break;

            case 3:mon=time_cell2();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",wednesday2[mon]);
                   break;

            case 4:mon=time_cell2();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",thursday2[mon]);
                   break;

            case 5:mon=time_cell2();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",friday2[mon]);
                   break;
            case 6:mon=time_cell2();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",saturday2[mon]);
                   break;
            default:printf("Wrong value entered sorry!!");

    }
    prints1();
    printf("\t \t \t \t \t \t \t \t \t    UG2 \n");
    prints1();
    printf("S NO.|"); printf("Timings           |"); printf("        Monday         |");printf("       Tuesday         |");printf("      Wednesday        |");printf("       Thursday        |");printf("        Friday         |");printf("      Saturday      |\n");
    prints1();

    for(int i=0;i<15;i++)
   {    printf("%-5s|",sn2[i]);
        printf("%s       |",time2[i]);
        printf("%-20s   |",monday2[i]);
        printf("%-20s   |",tuesday2[i]);
        printf("%-20s   |",wednesday2[i]);
        printf("%-20s   |",thursday2[i]);
        printf("%-20s   |",friday2[i]);
        printf("%-20s|",saturday2[i]);
        printf("\n");
        if((i==1)||(i==3)||(i==4)||(i==9)||(i==8) || (i==6) || (i==14))
            {  prints1(); }
    }
break;
}

/**ug3 case starting ------------------------------------------------------------------------------------------------------------*/
case 3:
    { printf("Enter\n");
            printf("1 for Monday \n2 for Tuesday \n3 for Wednesday \n4 for Thursday \n5 for Friday \n6 for Saturday \n");
            printf("Please Enter Your Choice   ");
            int z=0,mon;
            scanf("%d",&z);

        while((z<1)||(z>6))
        {    printf("Please reenter your choice!!  ");
             scanf("%d",&z);
        }
         switch(z)
        {
            case 1:mon=time_cell3();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",monday3[mon]);
                   break;

            case 2:mon=time_cell3();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",tuesday3[mon]);
                   break;

            case 3:mon=time_cell3();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",wednesday3[mon]);
                   break;

            case 4:mon=time_cell3();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",thursday3[mon]);
                   break;

            case 5:mon=time_cell3();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",friday3[mon]);
                   break;
            case 6:mon=time_cell3();
                   printf("Enter the new subject or film to watch \t");
                   scanf(" %[^\n]",saturday3[mon]);
                   break;
            default:printf("Wrong value entered sorry!!");

    }
    prints1();
    printf("\n\t \t \t \t \t \t \t \t \tUG-3 / UG-4 / MS / PhD \n");
    prints1();
    printf("S NO.|"); printf("Timings           |"); printf("        Monday         |");printf("       Tuesday         |");printf("      Wednesday        |");printf("       Thursday        |");printf("        Friday         |");printf("      Saturday      |\n");
    prints1();

    for(int i=0;i<18;i++)
   {    printf("%-5s|",sn3[i]);
        printf("%s       |",time3[i]);
        printf("%-20s   |",monday3[i]);
        printf("%-20s   |",tuesday3[i]);
        printf("%-20s   |",wednesday3[i]);
        printf("%-20s   |",thursday3[i]);
        printf("%-20s   |",friday3[i]);
        printf("%-20s|",saturday3[i]);
        printf("\n");
         if((i==1)||(i==3)||(i==4)||(i==6)||(i==8)||(i==9)||(i==11)||(i==13)||(i==15))
        {  for(int j=1;j<165;j++)
           {printf("_");}
            printf("\n");
        }

   }
   prints1();
break;}

}// switch closed ug1 ug 2ug 3
}// if over
}// while loop over
}// main over



int time_cell1()
{int x=100;          // INPUT CONTROL
                   while((x>13)||(x==5)||(x==8))
                   {
                        printf("Enter values upto 13 only ( 5th period is short break and 8th period is lunch)\n");
                        scanf("%d",&x);
                    }
                    return(x-1);
}

int time_cell2()
{int x=100;          // INPUT CONTROL
                   while((x>15)||(x==5)||(x==10))
                   {
                        printf("Enter values upto 15 only ( 5th period is short break and 10th period is lunch) \n");
                        scanf("%d",&x);
                    }
                    return(x-1);
}

int time_cell3()
{int x=100;          // INPUT CONTROL
                   while((x>18)||(x==5)||(x==10))
                   {
                        printf("Enter values upto 18 only ( 5th period is short break and 10th period is lunch) \n");
                        scanf("%d",&x);
                    }
                    return(x-1);
}

void prints1()
{
    for(int i=1;i<166;i++)
    {printf("_");}
    printf("\n");
}
void line()
{
  for (int l=1;l<=65;l++)
    {
        printf("_");
    }
    printf("\n");
}



